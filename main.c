#include "pico/stdlib.h"
#include <stdio.h>
#include <stdbool.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define BUTTON_PIN 5
#define LED1_PIN 11
#define LED2_PIN 12
#define LED3_PIN 13

volatile bool button_pressed = false;
volatile bool timer_running = false;
int led_state = 0;
int countdown = 3;

// Callback do temporizador que controla o desligamento sequencial dos LEDs
int64_t timer_callback(alarm_id_t id, void *user_data) {
    if (countdown > 0) {
        printf("LED%d off in %ds...\n", led_state + 1, countdown);
        countdown--;
        return 1000000;  // Reagenda o callback para 1 segundo depois
    } else {
        gpio_put(LED1_PIN + led_state, 0);  // Desliga o LED atual
        printf("LED%d OFF\n", led_state + 1);
        led_state++;

        if (led_state < 3) {  // Se ainda houver LEDs para desligar
            countdown = 3;    // Reinicia a contagem para o próximo LED
            return 1000;      // Chama o próximo callback após 1 ms para iniciar nova contagem
        } else {  // Todos os LEDs foram desligados
            printf("All LEDs OFF\n");
            led_state = 0;    // Reseta o estado para próxima execução
            timer_running = false;  // Permite nova interação com o botão
            return 0;  // Para o temporizador
        }
    }
}

// Função de debounce para evitar leituras falsas do botão
bool debounce() {
    static uint32_t last_time = 0;
    const uint32_t debounce_delay = 50;  // Tempo de debounce de 50 ms
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    if ((current_time - last_time) > debounce_delay) {
        last_time = current_time;
        return gpio_get(BUTTON_PIN) == 0;  // Retorna verdadeiro se o botão foi pressionado
    }
    return false;
}

// Interrupção chamada ao pressionar o botão
void button_isr(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN && !timer_running && debounce()) {
        button_pressed = true;  // Marca que o botão foi pressionado
    }
}

int main() {
    stdio_init_all();  // Inicializa a comunicação serial para debug

    // Configura o botão como entrada com pull-up interno
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // Inicializa os pinos dos LEDs como saída e garante que estão desligados
    gpio_init(LED1_PIN);
    gpio_init(LED2_PIN);
    gpio_init(LED3_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_set_dir(LED3_PIN, GPIO_OUT);
    gpio_put(LED1_PIN, 0);
    gpio_put(LED2_PIN, 0);
    gpio_put(LED3_PIN, 0);

    // Configura a interrupção para o botão (detecção de borda de descida)
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    while (1) {
        if (button_pressed && !timer_running) {
            button_pressed = false;  // Reseta o estado do botão
            timer_running = true;    // Inicia o controle do temporizador
            led_state = 0;
            countdown = 3;

            // Liga todos os LEDs
            gpio_put(LED1_PIN, 1);
            gpio_put(LED2_PIN, 1);
            gpio_put(LED3_PIN, 1);
            printf("All LEDs ON\n");

            // Inicia o temporizador para desligar os LEDs sequencialmente
            add_alarm_in_ms(1, timer_callback, NULL, false);
        }
    }
    return 0;
}
