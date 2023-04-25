START - indicates where your main function starts
END - indicates where your main function ends.
ADD *variable* *value*/*variable*- adds the second value/variable to the first variable
SUBSTRACT *variable* *value*/*variable*- substracts the second value/variable from the first variable
MULTIPLY *variable* *value*/*variable*- multiplies the second value/variable with the first variable
DIVIDE *variable* *value*/*variable*- divides first variable with the second value/variable
PRINT *variable*/"*string*"- writes the value/variable to the console
IF (expression) - if body should be implemented by starting every row with a '\t'. Multiple if bodies embedded into each other should use multiple '\t'. If you use less '\t', it means you exited the if body.
JUMP "value"- jumps to the row of the "value" parameter. The first line is identified with '1'
CREATE *variable name* *value* - Creates a variable with the given name. The value of the variable will be the given number (integer or float) or the string value inside the ' " '. If the value is not a number or between two ", zou will get a nice exception.
FUNC *function name* - initializes a function indicated by the function name. Function body should follow. It ends with a blank line.
CALL *function name* - Calls a function identified by the function name. Function should be declared BEFORE the call command. To use return value use the following syntax: a = CALL function(params)
RETURN value - returns from the function with a value. Cannot be expression (a + b).

It is now possible to write inline expression such as: a = b*c+2 * (3+3). Note thet all expression will be executed in the order it is written except if you use '(' and ')'. (meaning 'a + b - a * b' will be interpreted so the '+' will be executed first, except if you write 'a + b - (a*b)')

You can call predefined external functions. To register a function use: virualMachine.AddExternalFunction(name, pointer). You can only register either void function with no parameter, one string parameter or two float parameters. These functions should be called the same way as internal functions.

You can create structs from the C++ code: VirtualMachine.initializeStruct(name); VirtualMachine.addFieldToStruct(structName, fieldName, value); VirtualMachine.addStructFieldToStruct(structNameToAddTo, fieldName, structNameToBeAdded);

Get field values with getNumberField(name) and getStringField(name) functions.

Set struct field values with setStructField(name, value) functions