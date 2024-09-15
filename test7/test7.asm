;COMPUTES THE MAX OF FOOUR NUMBERS
var1: data 45
var2: data 823
var3: data 1245
var4: data 16
ldc var1
ldc var2
sub
brlz second
ldc var1
stl 10
br secondcomp
second: ldc var2
stl 10
secondcomp:
ldc var3
ldc var4
sub
brlz third
ldc var3
stl 5
br thirdcomp
third: ldc var4
stl 5
thirdcomp:
ldl 10             
ldl 5              
sub                 
brlz fourth
ldl 10
HALT
fourth: ldl 5
HALT