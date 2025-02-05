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

int64_t timer_callback(alarm_id_t id, void *user_data) {
    if (countdown > 0) {
        printf("LED%d off in %ds...\n", led_state + 1, countdown);
        countdown--;
        return 1000000;
    } else {
        gpio_put(LED1_PIN + led_state, 0);
        printf("LED%d OFF\n", led_state + 1);
        led_state++;

        if (led_state < 3) {
            countdown = 3;
            return 1000;
        } else {
            printf("All LEDs OFF\n");
            led_state = 0;
            timer_running = false;
            return 0;
        }
    }
}

bool debounce() {
    static uint32_t last_time = 0;
    const uint32_t debounce_delay = 50;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if ((current_time - last_time) > debounce_delay) {
        last_time = current_time;
        return gpio_get(BUTTON_PIN) == 0;
    }
    return false;
}

void button_isr(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN && !timer_running && debounce()) {
        button_pressed = true;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_init(LED1_PIN);
    gpio_init(LED2_PIN);
    gpio_init(LED3_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_set_dir(LED3_PIN, GPIO_OUT);

    gpio_put(LED1_PIN, 0);
    gpio_put(LED2_PIN, 0);
    gpio_put(LED3_PIN, 0);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    while (1) {
        if (button_pressed && !timer_running) {
            button_pressed = false;
            timer_running = true;
            led_state = 0;
            countdown = 3;

            gpio_put(LED1_PIN, 1);
            gpio_put(LED2_PIN, 1);
            gpio_put(LED3_PIN, 1);
            printf("All LEDs ON\n");

            add_alarm_in_ms(1, timer_callback, NULL, false);
        }
    }
    return 0;
}
