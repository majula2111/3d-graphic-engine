#include <iostream>
#include <cstdlib>
#include <exception>

#include "first_app.h"

int main() {
    lve::FirstApp firstApp{};

    try {
        firstApp.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
