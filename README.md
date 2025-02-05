# 📋 Descrição Geral

Este projeto implementa um **sistema de temporização para acionamento de LEDs** utilizando o microcontrolador **Raspberry Pi Pico W** e a função `add_alarm_in_ms()` do **Pico SDK**. O acionamento ocorre através de um **botão (pushbutton)**, alterando os estados dos LEDs com um atraso de **3 segundos** entre cada mudança.

---

# 🛠 Componentes Utilizados

- **Raspberry Pi Pico W** (Microcontrolador)
- **03 LEDs (azul, vermelho e verde)**
- **03 Resistores de 330 Ω**
- **Botão (Pushbutton)**

---

# 📂 Estrutura do Projeto

```plaintext
timer-oneshop/
├── wokwi
│   ├── diagram.json
│   ├── wokwi.toml
├── .gitignore
├── CMakeLists.txt
├── LICENSE
├── main.c
├── pico_sdk_import.cmake
└── README.md
```

---

# 🚀 Funcionalidades do Projeto

### 1. Estados dos LEDs:
   - **Clique no botão:** Todos os LEDs são acionados simultaneamente.
   - **Após 3 segundos:** Um LED é desligado.
   - **Após mais 3 segundos:** O segundo LED é desligado.
   - **Após mais 3 segundos:** O último LED é desligado.
   - **Novo clique no botão:** O ciclo reinicia.

### 2. Temporização:
   - Utiliza `add_alarm_in_ms()` para alternar os LEDs a cada **3 segundos**.
   - Bloqueia novas chamadas enquanto os LEDs ainda estão sendo desligados.
   - Implementa debounce (opcional) para evitar acionamentos acidentais.

---

# 🔧 Requisitos Técnicos

- **Precisão na temporização:** Uso de `add_alarm_in_ms()` para alternar os LEDs.
- **Call-back para alteração dos LEDs:** Implementação na função `turn_off_callback()`.
- **Bloqueio de acionamento:** O botão só pode ser pressionado após o desligamento do último LED.
- **Mensagens na Serial:** Impressão de status a cada mudança para facilitar o debug.
- **Código bem estruturado e comentado.**

---

# ⚙️ Instalação e Execução

### 1. Configuração do Ambiente

- Certifique-se de que o **Pico SDK** está instalado e configurado corretamente.
- Instale as dependências necessárias para compilação do código.

### 2. Clonando o Repositório

```bash
git clone https://github.com/brunoschumacherf/timer-oneshot
```

### 3. Compilando e Enviando o Código

```bash
mkdir build
cd build
cmake ..
make
```

---

_Desenvolvido por Bruno Schumacher_

