//cx is an address which RAM is free after
push 0
pop cx


push 2
pop cx

//___main___//
in 
pop [cx + 4]//avokado
in 
pop [cx + 5]//baunti
in 
pop [cx + 6]//specii

//assignment//

//function_call
push cx 
pop dx 
push 9 
pop cx


//initialize arguments
push [cx + 4]//avokado
pop ax

push ax
pop [cx + 0]

push [cx + 5]//baunti
pop ax

push ax 
pop [cx + 1]

push [cx + 6]//specii
pop ax

push ax 
pop [cx + 2]

call oven:
push dx 
pop cx

push ax
pop [cx + 2]//dish
//end of assignment//


//if-expression//

//(condition)//
push 0//const 
pop ax

push ax//to stack

push [cx + 2]//dish
pop ax

push ax//to stack

jne label_0:
//(condition)//
push [cx + 2]//dish
pop ax

push ax
out
label_0:
//end of if-expression//

//if-expression//

//(condition)//
push 1//const 
pop ax

push ax//to stack

push [cx + 2]//dish
pop ax

push ax//to stack

jne label_1:
//(condition)//
push [cx + 2]//dish
pop ax

push ax
out
push [0]//Korenber
pop ax

push ax
out
label_1:
//end of if-expression//

//if-expression//

//(condition)//
push 2//const 
pop ax

push ax//to stack

push [cx + 2]//dish
pop ax

push ax//to stack

jne label_2:
//(condition)//
push [cx + 2]//dish
pop ax

push ax
out
push [0]//Korenber
pop ax

push ax
out
push [1]//Korenyasen
pop ax

push ax
out
label_2:
//end of if-expression//

//if-expression//

//(condition)//
push 3//const 
pop ax

push ax//to stack

push [cx + 2]//dish
pop ax

push ax//to stack

jne label_3:
//(condition)//
push 888//const 
pop ax

push ax
out
label_3:
//end of if-expression//
end

//___end_of_main___//


//___function mixer___//
mixer:

//if/else-expression//

//(condition)//
push 0//const 
pop ax

push ax//to stack

push [cx + 0]//baunti
pop ax

push ax//to stack

jne label_4:
//(condition)//

//if/else-expression//

//(condition)//
push 0//const 
pop ax

push ax//to stack

push [cx + 1]//specii
pop ax

push ax//to stack

jne label_5:
//(condition)//
push 3//const 
pop ax

jmp label_6:
label_5:
push 0//const 
pop ax

label_6:
//end of if/else-expression//
jmp label_7:
label_4:

//assignment//
push -1//const
pop ax

push ax//to stack

push [cx + 1]//specii
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

push [cx + 0]//baunti
pop ax

push ax//to stack

div
pop ax

push ax
pop [0]//Korenber
//end of assignment//

push 1//const 
pop ax

label_7:
//end of if/else-expression//
ret

//___end of function mixer___//


//___function oven___//
oven:

//if/else-expression//

//(condition)//
push 0//const 
pop ax

push ax//to stack

push [cx + 0]//avokado
pop ax

push ax//to stack

jne label_8:
//(condition)//

//function_call
push cx 
pop dx 
push 5 
pop cx


//initialize arguments
push [cx + 1]//baunti
pop ax

push ax 
pop [cx + 0]

push [cx + 2]//specii
pop ax

push ax 
pop [cx + 1]

call mixer:
push dx 
pop cx

jmp label_9:
label_8:

//assignment//
push [cx + 1]//baunti
pop ax

push ax//to stack

push [cx + 1]//baunti
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

push 4//const
pop ax

push ax//to stack

push [cx + 2]//specii
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

push [cx + 0]//avokado
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

sub
pop ax

push ax
pop [cx + 3]//Doshirak
//end of assignment//


//if-expression//

//(condition)//
push 0//const 
pop ax

push ax//to stack

push [cx + 3]//Doshirak
pop ax

push ax//to stack

jae label_10:
//(condition)//
push 0//const 
pop ax

label_10:
//end of if-expression//

//if-expression//

//(condition)//
push 0//const 
pop ax

push ax//to stack

push [cx + 3]//Doshirak
pop ax

push ax//to stack

jne label_11:
//(condition)//

//assignment//
push -1//const
pop ax

push ax//to stack

push [cx + 1]//baunti
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

push 2//const
pop ax

push ax//to stack

push [cx + 0]//avokado
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

div
pop ax

push ax
pop [0]//Korenber
//end of assignment//

push 1//const 
pop ax

label_11:
//end of if-expression//

//assignment//
push -1//const
pop ax

push ax//to stack

push [cx + 1]//baunti
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

push [cx + 3]//Doshirak
pop ax

push ax
sqrt
pop ax

push ax//to stack

sub
pop ax

push ax//to stack

push 2//const
pop ax

push ax//to stack

push [cx + 0]//avokado
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

div
pop ax

push ax
pop [0]//Korenber
//end of assignment//


//assignment//
push -1//const
pop ax

push ax//to stack

push [cx + 1]//baunti
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

push [cx + 3]//Doshirak
pop ax

push ax
sqrt
pop ax

push ax//to stack

add
pop ax

push ax//to stack

push 2//const
pop ax

push ax//to stack

push [cx + 0]//avokado
pop ax

push ax//to stack

mul
pop ax

push ax//to stack

div
pop ax

push ax
pop [1]//Korenyasen
//end of assignment//

push 2//const 
pop ax

label_9:
//end of if/else-expression//
ret

//___end of function oven___//
