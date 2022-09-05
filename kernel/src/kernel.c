#include <stdint.h>
#include <stddef.h>
#include <limine.h>

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.


static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0
};

static void done(void) {
    for (;;) {
        __asm__("hlt");
    }
}

int stringlen(const char *string) {
    int len;
    for (len = 0; string[len] != 0; len++);
    return len;
}

#define STRING(str) str, stringlen(str)


// The following will be our kernel's entry point.
void _start(void) {
    // Ensure we got a terminal
    if (terminal_request.response == NULL
     || terminal_request.response->terminal_count < 1) {
        done();
    }

    // We should now be able to call the Limine terminal to print out
    // a simple "Hello World" to screen.
    struct limine_terminal *terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, STRING("Hello, world!\n"));

    if (module_request.response == NULL
        || module_request.response->module_count < 1) {
        terminal_request.response->write(terminal, STRING("No modules found.\n"));
        done();
    }

    terminal_request.response->write(terminal, STRING("Modules:\n"));
    for (int i = 0; i < module_request.response->module_count; i++) {
        struct limine_file *module = module_request.response->modules[i];
        terminal_request.response->write(terminal, STRING(module->path));
        terminal_request.response->write(terminal, STRING("\n"));
    } 

    // We're done, just hang...
    done();
}
