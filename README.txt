START - indicates where your main function starts
END - indicates where your main function ends.
ADD *variable* *value*/*variable*- adds the second value/variable to the first variable
SUBSTRACT *variable* *value*/*variable*- substracts the second value/variable from the first variable
MULTIPLY *variable* *value*/*variable*- multiplies the second value/variable with the first variable
DIVIDE *variable* *value*/*variable*- divides first variable with the second value/variable
PRINT *variable*/"*string*"- writes the value/variable to the console
IF expression - if the evaluation is true it executes the next row (usually a jump), skips the next row otherwise. Use space after every variable in the expression (TODO).
JUMP "value"- jumps to the row of the "value" parameter. The first line is identified with '1'
CREATE *variable name* *value* - Creates a variable with the given name. The value of the variable will be the given number (integer or float) or the string value inside the ' " '. If the value is not a number or between two ", zou will get a nice exception.
FUNC *function name* - initializes a function indicated by the function name. Function body should follow. It ends with a blank line.
CALL *function name* - Calls a function identified by the function name. Function should be declared BEFORE the call command. To use return value use the following syntax: a = CALL function(params)
RETURN value - returns from the function with a value. Cannot be expression (a + b).

You can call predefined external functions. To register a function use: virualMachine.AddExternalFunction(name, pointer). You can only register void function with no parameter, one string parameter, two float parameters. These functions should be called the same way as internal functions.