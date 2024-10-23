#include "json.h"
#include <stdio.h>

int main() {
    struct JsonParser parser;
    char buffer[32];
    init_json_parser(&parser, "{\"somekey\": [\"alpha\", \"beta\", \"omega\"]}");

    while (json_parser_has_next(&parser)) {
        enum JsonEvent event = json_parser_next(&parser);
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
            case KEY_STRING:
                printf("key : ");
                json_parser_get_string(&parser, buffer, 31);
                printf("%s\n", buffer);
                break;
            case VALUE_STRING:
                printf("val : ");
                json_parser_get_string(&parser, buffer, 31);
                printf("%s\n", buffer);
                break;
        }
    }
}