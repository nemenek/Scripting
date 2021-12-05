START - indicates where your main function starts
END - indicates where your main function ends.
ADD *variable* *value*/*variable*- adds the second value/variable to the first variable
SUBSTRACT *variable* *value*/*variable*- substracts the second value/variable from the first variable
MULTIPLY *variable* *value*/*variable*- multiplies the second value/variable with the first variable
DIVIDE *variable* *value*/*variable*- divides first variable with the second value/variable
PRINT *variable*/"*value*"- writes the value/variable to the console
IF expression - if the evaluation is true it executes the next row (usually a jump), skips the next row otherwise
JUMP "value"- jumps to the row of the "value" parameter. The first line is identified with '1'
CREATE *variable name* *value* - Creates a variable with the given name. The value of the variable will be the given number (integer or float) or the string value inside the ' " '. If the value is not a number or between two ", zou will get a nice exception.
FUNC *function name* - indicates where the function starts. The function should end with a jump for now.
CALL *function name* - Calls a function identified by the function name. Function should be declared BEFORE the call command.