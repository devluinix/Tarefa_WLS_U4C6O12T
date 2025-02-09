#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "inc/ssd1306.h"
#include "pico/time.h"
#include "inc/font.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define BUTTON_A 5
#define LED_BLUE 11
#define LED_GREEN 12

#define BUTTON_B 6
#define endereco 0x3C

bool updateDisplay = true;
absolute_time_t next_update = 0;

bool ledBlue = false;
bool ledGreen = false;

char charDigitado = 'A';

void button_irq_handler(uint gpio, uint32_t events)
{
  absolute_time_t now = get_absolute_time();
  if (absolute_time_diff_us(next_update, now) < 0)
  {
    return;
  }
  next_update = delayed_by_us(now, 200000);
  printf("Botão pressionado no pino %d\n", gpio);
  if (gpio == BUTTON_A)
  {
    printf("Botão A\n");
    gpio_put(LED_BLUE, ledBlue = !ledBlue);
    updateDisplay = true;
  }
  else
  {
    gpio_put(LED_GREEN, ledGreen = !ledGreen);
    printf("Botão B\n");
    updateDisplay = true;
  }
}

int main()
{
  stdio_init_all();
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  //============== INICIALIZAÇÃO DISPLAY
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                                        // Pull up the data line
  gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
  ssd1306_t ssd;                                                // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display
  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  //============== INICIALIZAÇÃO BOTÕES
  gpio_init(BUTTON_A);
  gpio_init(BUTTON_B);
  gpio_set_dir(BUTTON_A, GPIO_IN);
  gpio_set_dir(BUTTON_B, GPIO_IN);
  gpio_pull_up(BUTTON_A);
  gpio_pull_up(BUTTON_B);

  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
  gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

  //============== INICIALIZAÇÃO LEDS
  gpio_init(LED_BLUE);
  gpio_init(LED_GREEN);
  gpio_set_dir(LED_BLUE, GPIO_OUT);
  gpio_set_dir(LED_GREEN, GPIO_OUT);
  gpio_put(LED_BLUE, 0);
  gpio_put(LED_GREEN, 0);

  //============== INICIALIZAÇÃO UART

  printf("Bem vindo a Atividade U4C6O12T!\n");
  while (true)
  {
    if (updateDisplay || scanf(" %c", &charDigitado))
    {
      printf("Atualizando display\n");
      updateDisplay = false;
      ssd1306_fill(&ssd, false); // Limpa o display
      char buffer[15];
      char buffer2[14];
      sprintf(buffer, "Led: G:%d, B:%d", ledGreen, ledBlue);
      if (!charDigitado)
        charDigitado = ' ';
      sprintf(buffer2, "[%c] - U4C6O12T", charDigitado);
      ssd1306_draw_string(&ssd, buffer2, 8, 10);         // Desenha uma string
      ssd1306_draw_string(&ssd, buffer, 10, 25);         // Desenha uma string
      ssd1306_draw_string(&ssd, "Aluno Luinix", 15, 48); // Desenha uma string
      ssd1306_send_data(&ssd);                           // Atualiza o display
    }
    sleep_ms(200);
  }
}
