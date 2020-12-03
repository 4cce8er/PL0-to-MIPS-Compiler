# PL0-to-MIPS-Compiler
Modified a PL/0 compiler to generate MIPS assembly language.  
  
PL/0 is a simple language mainly used for teaching. Its programs are executed in a JAVA-like compile-interpret way. The compiler will generate intermediate code (P-code) and then the interpreter runs it, in a stack-based way.

This modification code inserted some parts in the compiler to generate MIPS instructions. MIPS is mainly register-based but when parsing expressions, the compiler still used stack (and the stack pointer register in MIPS structure) so as to implement it in an easier way. 

## Clarification
I only did some modification on the PL/0 compiler source code.  
This is a course project and the original compiler code is provided by our teacher; its code is probably from our textbook.  
Plus, this is why you can see different coding style in the file.  
  
If you find this code infriging your copyright, please contact me and I'll delete it.  

## How To Run
You should write your PL/0 source code as a .pl0 file. Examples are provided (so is the MIPS output file for these examples).  
Run the program and enter the file name of your PL/0 source code.  
Answer if you want to print the object code.  
Answer if you want to print the symbol table.  

After you can find your files as below:
fa.tmp 			P-code virtual machine code  
fa1.tmp  		source file and the address that its each line corresponds to  
fa2.tmp  		result  
fas.tmp  		symbol table  
fa3_data.s 	MIPS assembly language code  

The outputted MIPS assembly language code is expected to run on a QtSPIM simulator, which you can sue to run tests.
  
I did not delete its original function code so you can see the P-code file is also outputted.