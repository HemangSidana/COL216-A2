main:
  addi $1, $0 , 2
  addi $2, $0, 1
  sw $1, 1024($2)
  add $1 , $1 , $1
  sub $2 , $1 , $2
  lw  $3 , 1024($2)
exit: