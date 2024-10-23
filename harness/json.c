#include <stdio.h>
#include <stdlib.h>
#include "../json.h"

#define TEST_BUF_SIZE (1024 * 20) // 20kB max test size
#define MAX_STRING_SIZE 1024

void print_event(struct JsonParser* parser, enum JsonEvent event) {
    char stringbuf[MAX_STRING_SIZE]; // for reading string values

    switch (event) {
        case OBJECT_BEGIN:
            printf("OBJECT_BEGIN\n");
            break;
        case OBJECT_END:
            printf("OBJECT_END\n");
            break;
        case ARRAY_BEGIN:
            printf("ARRAY_BEGIN\n");
            break;
        case ARRAY_END:
            printf("ARRAY_END\n");
            break;
        case VALUE_NULL:
            printf("VALUE_NULL\n");
            break;
        case KEY_STRING:
            json_parser_get_string(&parser, stringbuf, MAX_STRING_SIZE - 1);
            printf("KEY_STRING=%s", stringbuf);
            break;
        case VALUE_STRING:
            json_parser_get_string(&parser, stringbuf, MAX_STRING_SIZE - 1);
            printf("VALUE_STRING=%s", stringbuf);
            break;
        default:
            printf("EVENT #%i\n", event);
    }
}

int main() {
    char buf[TEST_BUF_SIZE]; // for the test input
    
    read(0, buf, TEST_BUF_SIZE - 1);

    struct JsonParser parser;
    json_parser_init(&parser, buf);

    while (json_parser_has_next(&parser)) {
        enum JsonEvent event = json_parser_next(&parser);
        print_event(&parser, event);
    }
}