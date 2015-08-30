#include <stdio.h>
#include "Common.h"

int main(int argc, char **argv)
{
    Config config;
    parse_args(&config, argc, argv);

    printf("Data size: %dx%dx%d (%d objects, %zu bytes)\n",
        config.data_size, config.data_size, config.data_size,
        volume(Vec3i(config.data_size)),
        volume(Vec3i(config.data_size)) * sizeof(Sphere));

    do_arrays(config);
    do_chunks(config);
    return 0;
}
