#include <hardware/pio.h>
#include <tusb.h>
#include <cstdio>

#include "led.pio.h"

namespace {

#define pio pio0
int sm;
void setup_led_pio() {
    sm = pio_claim_unused_sm(pio, true);
    auto offset = pio_add_program(pio, &led_program);

    pio_sm_config c = led_program_get_default_config(offset);
    sm_config_set_clkdiv(&c, 1);

    sm_config_set_sideset_pin_base(&c, PICO_DEFAULT_LED_PIN);
    pio_gpio_init(pio, PICO_DEFAULT_LED_PIN);
    pio_sm_set_consecutive_pindirs(pio, sm, PICO_DEFAULT_LED_PIN, 1, true);

    // Auto pull/push
    sm_config_set_in_shift(&c, true, true, 32);
    sm_config_set_out_shift(&c, true, true, 32);

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
    pio_sm_drain_tx_fifo(pio, sm);
}

} // namespace {

int main()
{
    stdio_init_all();
    while (!tud_cdc_connected())
        sleep_ms(100);

    puts("\033[2J\033[HHello there !\n");

    setup_led_pio();

    uint32_t data = 0;
    while(true) {
        sleep_ms(500); // at this point the led should be ON for 500ms
        pio_sm_put_blocking(pio, sm, data++);

        sleep_ms(500); // at this point the led should be OFF for 500ms
        uint32_t dat = pio_sm_get_blocking(pio, sm);
        printf("%lx\n", dat);
    }
    return 0;
}
