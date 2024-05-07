# XML to JSON conversion for SQLite3

This is an experimental extension that adds support for converting XML to JSON
to SQLite3.

## Installation

This will compile sqlite3 and the extension:

```
$ make extension
```

## Testing

```
$ make test
```

## Usage

This will compile sqlite3 and the extension:

```
$ dist/sqlite3
```

Next, load the extension:

```
sqlite> .load dist/xml_to_json.dylib
```

Convert XML to JSON using SQL:

```
sqlite> select xml_to_json('<A><B>C</B></A>');
```
