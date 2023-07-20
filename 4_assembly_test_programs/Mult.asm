// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.
  @R2
  M=0

  @R0
  D=M
  @END
  D;JEQ

  @var1
  M=D

  @R1
  D=M
  @END
  D;JEQ

  @var2
  M=D

  @prod
  M=0

  @i
  M=0

(LOOP)
  @var2
  D=M
  @i
  D=D-M
  @STOP
  D;JEQ


  @prod
  D=M
  @var1
  D=D+M
  @prod
  M=D
  @i
  M=M+1
  @LOOP
  0;JMP

(STOP)
  @prod
  D=M
  @R2
  M=D

(END)
  @END
  0;JMP
