#include <Application.hpp>
#include <csignal>
#include <memory>
#include <signal.h>
#include <stdio.h>

void millisecond_delay(int ms) {
    for(int i = 0; i < ms; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

std::unique_ptr<Application> app;
void onSig(int) {
    printf("\nClosing...\n");
    if(app != nullptr) {
        app->setShouldExit();
    }
}

int main() {
    app = std::make_unique<Application>();

    signal(SIGINT, onSig);
    while(!app->getShouldExit()) {
        millisecond_delay(200);
    }

    app.reset();

    return 0;
}