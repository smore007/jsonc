#include <string.h>
#include <stdbool.h>

#ifndef JSON_PARSER_MAX_DEPTH
#define JSON_PARSER_MAX_DEPTH 1024
#endif

enum JsonEvent {
    OBJECT_BEGIN,
    OBJECT_END,
    
    ARRAY_BEGIN,
    ARRAY_END,

    KEY_STRING,
    VALUE_STRING,
    VALUE_NUMBER,
    VALUE_BOOLEAN,
    VALUE_NULL
};

enum JsonParserState {
    IN_OBJECT,
    IN_ARRAY,
    IN_VALUE
};

struct JsonParser {
    char* last_position;
    char* current_position;

    struct JsonParserStateStack {
        enum JsonParserState stack[JSON_PARSER_MAX_DEPTH]; // INVARIANT: only contains OBJECT_BEGIN, ARRAY_BEGIN
        int size;
    } state;
};

void init_json_parser(struct JsonParser *parser, char *input) {
    parser->state.size = 0;
    parser->current_position = input;
    parser->last_position = parser->current_position;
}

void json_parser_seek_whitespace(struct JsonParser *parser) {
    // TODO: Handle all whitespaces, add strict vs non-strict mode maybe?
    while(*(parser->current_position) == ' ' || *(parser->current_position) == ',' || *(parser->current_position) == ':') {
        parser->current_position++;
    }
    // printf("current pos at end of seek_whitespace: %c\n", *parser->current_position);
}

unsigned json_parser_string_len(char* string_begin) {
    char* string_end = string_begin;
    
    while(*(++string_end) != '\0') {
        if (*(string_end) == '"') {
            // Unmoved needle
            if (string_begin == string_end) {
                continue;
            }

            // Check if quote was escaped
            if (*(string_end - 1) == '\\') {
                continue;
            }

            break;
        }
    }

    // printf("\nstrlen: %i\n", string_end - string_begin);

    return string_end - string_begin;
}

void json_parser_seek_end_of_string(struct JsonParser *parser) {
    parser->current_position += json_parser_string_len(parser->current_position) + 1;
}

void json_parser_state_push(struct JsonParser *parser, enum JsonParserState state) {
    parser->state.stack[parser->state.size++] = state;
}

enum JsonParserState json_parser_state_peek(struct JsonParser *parser) {
    return parser->state.stack[parser->state.size - 1];
}

enum JsonParserState json_parser_state_pop(struct JsonParser *parser) {
    return parser->state.stack[--parser->state.size];
}

bool json_parser_has_next(struct JsonParser *parser) {
    json_parser_seek_whitespace(parser);
    return parser->current_position != 0 && *parser->current_position != '\0';
}

enum JsonEvent json_parser_next(struct JsonParser *parser) {
    json_parser_seek_whitespace(parser);
    parser->last_position = parser->current_position;

    enum JsonEvent event;

    switch (*(parser->current_position)) {
        case '{':
            event = OBJECT_BEGIN;
            break;
        case '}':
            event = OBJECT_END;
            break;
        case '[':
            event = ARRAY_BEGIN;
            break;
        case ']':
            event = ARRAY_END;
            break;
        case '"':
        default:
            // one of:
            // - key name
            // - value
            enum JsonParserState top = json_parser_state_peek(parser);
            
            switch (top) {
                case IN_OBJECT:
                    event = KEY_STRING;
                    break;
                case IN_VALUE:
                case IN_ARRAY:
                    // value is one of:
                    // - value string ("*")
                    // - value boolean ("true"|"false")
                    // - value null ("null")
                    if (strncmp(parser->current_position, "\"true\"", strlen("\"true\"")) == 0
                    || strncmp(parser->current_position, "\"false\"", strlen("\"false\"")) == 0) {
                        event = VALUE_BOOLEAN;
                    } else if (strncmp(parser->current_position, "\"null\"", strlen("\"null\"")) == 0) {
                        event = VALUE_NULL;
                    } else {
                        event = VALUE_STRING;
                    }
                    break;
            }
            break;
    };
    
    switch (event) {
        case OBJECT_BEGIN:
            parser->current_position++;
            json_parser_state_push(parser, IN_OBJECT);
            break;
        case ARRAY_BEGIN:
            parser->current_position++;
            json_parser_state_push(parser, IN_ARRAY);
            break;
        case OBJECT_END:
        case ARRAY_END:
            parser->current_position++;
            json_parser_state_pop(parser);
            break;
        case KEY_STRING:
            json_parser_seek_end_of_string(parser);
            json_parser_state_push(parser, IN_VALUE);
            break;
        case VALUE_BOOLEAN:
        case VALUE_NULL:
        case VALUE_STRING:
            json_parser_seek_end_of_string(parser);
            json_parser_state_pop(parser);
            break;
    }

    return event;
}

void json_parser_get_string(struct JsonParser *parser, char* dest, unsigned max_size) {
    unsigned string_size = json_parser_string_len(parser->last_position + 1);
    if (string_size > max_size) {
        string_size = max_size;
    }

    strncpy(dest, parser->last_position + 1, string_size);
    dest[string_size] = '\0';
}