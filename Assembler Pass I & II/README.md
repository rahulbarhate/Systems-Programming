# Pass I And II of the Assembler

## Pass I (Analysis)

1. Seperate the labels, mnemonic opcode and operand fields of a statement.
2. Validate the mnemonic opcode through opcode table.
3. Build the Symbol Table & Literal Table.
4. Perform LC processing.
5. Generate the intermediate code.

## Pass II (Synthesis)

1. Takes the address of symbols from the Symbol Table, addresses of litreals from literal table and addresses of opcodes from opcode table.
2. Synthesize the target program.


* ```asm.txt``` contains the assembly language code.

* ```intermediate.txt``` contains the intermediate code as an output of pass I.

* ```output.txt``` contains the target code as on output of pass II.



## Instructions

### To Run:
> gcc pass1_2.c

> ./a.out

Contents of asm.txt (Assembly language code)

![Image of asm.txt](https://github.com/rahulbarhate/Systems-Programming/blob/master/Assembler%20Pass%20I%20%26%20II/images/asm.png)

Contents of indermediate.txt (Intermediate code)

![Image of intermediate.txt](https://github.com/rahulbarhate/Systems-Programming/blob/master/Assembler%20Pass%20I%20%26%20II/images/intermediate.png)

Contents of target.txt (Target code)

![Image of target.txt](https://github.com/rahulbarhate/Systems-Programming/blob/master/Assembler%20Pass%20I%20%26%20II/images/target.png)

Contents of Symbol Table, Pool Table and Literal Table

![Image](https://github.com/rahulbarhate/Systems-Programming/blob/master/Assembler%20Pass%20I%20%26%20II/images/output.png)


