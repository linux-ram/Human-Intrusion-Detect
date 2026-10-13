/* Compile and run from the repository root:
 *   cc -std=c11 -Wall -Wextra -o /tmp/test_algorithm_constants tests/test_algorithm_constants.c
 *   /tmp/test_algorithm_constants
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_define(const char *src, const char *name, long *out)
{
    char needle[128];
    const char *hit;
    snprintf(needle, sizeof needle, "#define %s ", name);
    hit = strstr(src, needle);
    if (hit == NULL) {
        return 0;
    }
    return sscanf(hit + strlen(needle), "%ld", out) == 1;
}

int main(void)
{
    FILE *f = fopen("IntrusionDetectionAlgorithm.c", "r");
    long window = 0, bins = 0, slide = 0, channels = 0;
    char *src;
    long n;

    if (f == NULL) {
        fprintf(stderr, "run this test from the repository root\n");
        return 1;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        return 1;
    }
    n = ftell(f);
    rewind(f);
    src = malloc((size_t)n + 1);
    if (src == NULL || fread(src, 1, (size_t)n, f) != (size_t)n) {
        return 1;
    }
    src[n] = '\0';
    fclose(f);

    if (!read_define(src, "ANALOG_DET_WNDW", &window) ||
        !read_define(src, "BINS", &bins) ||
        !read_define(src, "TEMP_WINDOW", &slide) ||
        !read_define(src, "CHANNELS", &channels)) {
        fprintf(stderr, "missing algorithm constant\n");
        return 1;
    }
    free(src);

    if (window != 128 || bins != 7 || slide != 16 || channels != 3) {
        fprintf(stderr, "constants drifted: window=%ld bins=%ld slide=%ld channels=%ld\n",
                window, bins, slide, channels);
        return 1;
    }
    if ((window & (window - 1)) != 0 || (slide & (slide - 1)) != 0 || slide >= window) {
        fprintf(stderr, "Haar window constraints failed\n");
        return 1;
    }
    if ((1L << bins) != window) {
        fprintf(stderr, "BINS is not log2(ANALOG_DET_WNDW)\n");
        return 1;
    }
    return 0;
}
