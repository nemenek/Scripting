PUSH "value"- pushes float value to stack
ADD - adds the two last pushed value. 'add' removes the two value from the stack and pushes the result to the stack
SUBSTRACT - substracts the two last pushed value. 'substract' removes the two value from the stack and pushes the result to the stack
MULTIPLY - mulitplies the two last pushed value. 'multiply' removes the two value from the stack and pushes the result to the stack
DIVIDE - divides the two last pushed value. 'divide' removes the two value from the stack and pushes the result to the stack
WRITE - writes the last pushed value to the console
POP - pops the last pushed value to the console
IF "value" - compares the last two pushed value. If they are equivalent the program continues running, if they are not, the program will skip the next lines. The amount of lines to be skipped is identified by the "value" parameter
NIF "value" - compares the last two pushed value. If they are not equivalent the program continues running, if they are equivalent the program will skip the next lines. The amount of lines to be skipped is identified by the "value" parameter
JUMP "value"- jumps to the row of the "value" parameter. The first line is identified with '1'