## jsonc

An unfinished single-header zero-allocation JSON stream parsing library.

### Usage

Simply include `json.h`, create  a JsonParser struct, and initialize it with `json_parser_init(&parser, buffer)`. Read json by stepping through it, as follows:
- While `json_parser_has_next(&parser)`:
    - Get event with `json_parser_next(&parser)`
    - If desired, use appropriate getter based on event
        - ex: if `KEY_STRING` or `VALUE_STRING`, use `json_parser_get_string`
        - ex: if `VALUE_NUMBER`, use `json_parser_get_int` or `json_parser_get_float`

### Tests

Run `make test` in [harness/](harness/).

### Notes
- See [the test harness](harness/json.c) and the [tests](harness/tests/) outputs for a concrete notion of the ordering of events and expected behavior.
- Undefined behavior:
    - `json_parser_next` returns giberrish in cases where `json_parser_has_next` returns false (when the end of the input buffer is reached)
    - `json_parser_get_*` behavior is undefined for events that are not values, such as `OBJECT_BEGIN` and `ARRAY_END`