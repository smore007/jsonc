# jsonc

An unpolished single-header zero-allocation JSON stream parsing library.

## Usage

Include `json.h`, create a JsonParser and initialize with  `json_parser_init(&parser, buffer)`, providing a null-terminated string buffer. While `json_parser_has_next(&parser)`, use `json_parser_next(&parser)` to get the next event and step through the JSON input. After getting an event, using a `json_parser_get_*` will produce the last event's value.

## Events

- `OBJECT_BEGIN`
    - The beginning of an object, e.g. `{`
- `OBJECT_END`
    - The end of an object, e.g. `}`
- `ARRAY_BEGIN`
    - The beginning of an array, e.g. `[`
- `ARRAY_END`
    - The end of an array, e.g. `]`
- `KEY_STRING`
    - Key name
    - Get with `json_parser_get_string`
- `VALUE_STRING`
    - Value of arbritrary string
    - Get with `json_parser_get_string`
- `VALUE_NUMBER`
    - Value of integer or floating point number
    - Get with `json_parser_get_int` or `json_parser_get_float`
- `VALUE_BOOLEAN`
    - Value of "true" or "false"
    - Get with `json_parser_get_boolean`
- `VALUE_NULL`
    - Value of "null"

## Tests

Run `make test` in [harness/](harness/). The harness consumes a JSON input and emits the events and their values (if an appropriate). Test inputs and expected outputs are of the form `*-in.json` and `*-out.txt`, respectively.

## Notes
- See [the test harness](harness/json.c) and the [tests](harness/tests/) outputs for a concrete notion of the ordering of events and expected behavior.
- Undefined behavior:
    - `json_parser_next` returns garbage in cases where `json_parser_has_next` returns false (such as when the end of the input buffer is reached)
    - `json_parser_get_*` behavior is undefined for events that are not values, such as `OBJECT_BEGIN` and `ARRAY_END`