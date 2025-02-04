/*Atividade realizada em atendimento à proposta de Tarefa em 29/01/2025
                       Semáforo
Residente: Jackson da Silva Carneiro
*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

#define LED_PIN_RED 13      // led vermelho
#define LED_PIN_B_Y 12      //led-> *amarelo no wokwi* - *azul na BitDogLab*
#define LED_PIN_GREEN 11 //led verde

// Variáveis para controlar o estado dos LEDs e o tempo
volatile int led_state = 0; // 0: vermelho, 1: amarelo, 2: verde
volatile uint32_t timer_start_time;

void configura_GPIO(){ //CONFIGURAÇÃO DOS PINOS GPIO

    gpio_init(LED_PIN_RED);          // Inicializa o pino do LED VERMELHO
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);   // Configura como saída

    gpio_init(LED_PIN_B_Y);          // Inicializa o pino do LED AMARELO
    gpio_set_dir(LED_PIN_B_Y, GPIO_OUT);   // Configura como saída

    gpio_init(LED_PIN_GREEN);        // Inicializa o pino do LED VERDE
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);   // Configura como saída
}

bool repeating_timer_callback(struct repeating_timer *t){  //Função chamada pela interrupção por timer
    uint32_t current_time = time_us_32();

    switch (led_state) {  //recebe led_state=0 e inicia o funcionamento do semáforo && recede as atualizações de led_state
        case 0: // Vermelho
            gpio_put(LED_PIN_RED, true);        //liga led vermelho
            gpio_put(LED_PIN_B_Y, false);      //desliga led amarelo
            gpio_put(LED_PIN_GREEN, false);   //desliga led verde
            if (current_time - timer_start_time >= 3000000) { // 3 segundos
                led_state = 1;     //altera led_state para 1
                timer_start_time = current_time;
            }
            break;
        case 1:                               // Amarelo<wokwi>       || vermelho+verde=amarelo<BitDogLab>
            gpio_put(LED_PIN_RED, false);     //desliga led vermelho     //liga led vermelho
            gpio_put(LED_PIN_B_Y, true);   //liga led amarelo         //desliga led azul
            gpio_put(LED_PIN_GREEN, false); //desliga led verde        //liga led verde
            if (current_time - timer_start_time >= 3000000) { // 3 segundos
                led_state = 2;  //altera led_state para 2
                timer_start_time = current_time;
            }
            break;
        case 2: // Verde
            gpio_put(LED_PIN_RED, false);    //desliga led vermelho
            gpio_put(LED_PIN_B_Y, false);   //desliga led amarelo
            gpio_put(LED_PIN_GREEN, true); //liga led verde
            if (current_time - timer_start_time >= 3000000) { // 3 segundos
                led_state = 0;
                timer_start_time = current_time;
            }
            break;
    }
    return true; // Indica que o timer deve continuar repetindo
}

int main()
{
    stdio_init_all();
    configura_GPIO();

    struct repeating_timer timer;
    timer_start_time = time_us_32(); // Inicializa o tempo de início

    add_repeating_timer_ms(10, repeating_timer_callback, NULL, &timer); // Intervalo menor

    while (true) {
        printf("\t\tSEMAFORO EM FUNCIONAMENTO!!!\n\n");
        sleep_ms(1000);  //intervalo de impressão da mensagem no loop
    }
}