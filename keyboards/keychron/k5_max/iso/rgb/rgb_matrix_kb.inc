// !!! DO NOT ADD #pragma once !!! //

#ifdef RGB_MATRIX_ENABLE

RGB_MATRIX_EFFECT(REACTIVE_SOLID)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

extern volatile bool g_capslock;
extern volatile bool g_numlock;

// Provided by keymap.c
extern bool g_rgb_held[RGB_MATRIX_LED_COUNT];

static inline uint8_t lerp8(uint8_t a, uint8_t b, uint8_t t) {
    // t: 0..255 (0 -> a, 255 -> b)
    return a + (((int16_t)(b - a) * t) / 255);
}

static inline RGB rgb_lerp(RGB a, RGB b, uint8_t t) {
    RGB out = {
        .r = lerp8(a.r, b.r, t),
        .g = lerp8(a.g, b.g, t),
        .b = lerp8(a.b, b.b, t),
    };
    return out;
}

static bool REACTIVE_SOLID(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    const uint8_t num_led_index  = 37;
    const uint8_t caps_led_index = 62;

    const uint16_t base_hold_ticks = 500;

    uint16_t denom            = (uint16_t)rgb_matrix_config.speed + 1;
    uint32_t scaled_hold      = (uint32_t)base_hold_ticks * 128u;
    uint16_t white_hold_ticks = (uint16_t)(scaled_hold / denom);

    if (white_hold_ticks < 40)   white_hold_ticks = 40;
    if (white_hold_ticks > 2000) white_hold_ticks = 2000;

    uint16_t fade_ticks = (uint16_t)((uint32_t)white_hold_ticks / 2u);
    if (fade_ticks < 30)   fade_ticks = 30;
    if (fade_ticks > 2000) fade_ticks = 2000;

    HSV base_hsv = rgb_matrix_config.hsv;
    base_hsv.s   = 255;
    RGB base_rgb = hsv_to_rgb(base_hsv);

    HSV special_hsv = (HSV){
        .h = (uint8_t)(rgb_matrix_config.hsv.s + 128),
        .s = rgb_matrix_config.hsv.s == 0 ? 0 : 255,
        .v = 255,
    };

    RGB special_rgb = hsv_to_rgb(special_hsv);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        // Lock indicators (always forced, no smoothing)
        if (i == num_led_index && g_numlock) {
            rgb_matrix_set_color(i, special_rgb.r, special_rgb.g, special_rgb.b);
            continue;
        }
        if (i == caps_led_index && g_capslock) {
            rgb_matrix_set_color(i, special_rgb.r, special_rgb.g, special_rgb.b);
            continue;
        }

        if (i == num_led_index || i == caps_led_index) {
            if (g_rgb_held[i]) {
                rgb_matrix_set_color(i, special_rgb.r, special_rgb.g, special_rgb.b);
            } else {
                rgb_matrix_set_color(i, base_rgb.r, base_rgb.g, base_rgb.b);
            }
            continue;
        }

        // Normal keys: keep held keys solid special
        if (g_rgb_held[i]) {
            rgb_matrix_set_color(i, special_rgb.r, special_rgb.g, special_rgb.b);
            continue;
        }

        // Normal keys: reactive timer + smooth fade back to base
        uint16_t tick = UINT16_MAX;
        for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
            if (g_last_hit_tracker.index[j] == i && g_last_hit_tracker.tick[j] < tick) {
                tick = g_last_hit_tracker.tick[j];
                break;
            }
        }

        if (tick != UINT16_MAX) {
            if (tick <= white_hold_ticks) {
                rgb_matrix_set_color(i, special_rgb.r, special_rgb.g, special_rgb.b);
            } else if (tick <= (uint16_t)(white_hold_ticks + fade_ticks)) {
                uint16_t t      = (uint16_t)(tick - white_hold_ticks);
                uint8_t  blendt = (uint8_t)(((uint32_t)t * 255u) / (uint32_t)fade_ticks);
                RGB mixed = rgb_lerp(special_rgb, base_rgb, blendt);
                rgb_matrix_set_color(i, mixed.r, mixed.g, mixed.b);
            } else {
                rgb_matrix_set_color(i, base_rgb.r, base_rgb.g, base_rgb.b);
            }
        } else {
            rgb_matrix_set_color(i, base_rgb.r, base_rgb.g, base_rgb.b);
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // RGB_MATRIX_ENABLE