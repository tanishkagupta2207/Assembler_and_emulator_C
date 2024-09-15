;searching 8 returns address of memory having this value
var1: data 3
var2: data 8
var3: data 10
search: data 8
ldc var1
ldc 5
stnl 0                ;var1 stored at location 5  
ldc var2 
ldc 5
stnl 1                ;var2 stored at location 6 
ldc var3
ldc 5
stnl 2               ;var3 stored at location 7 
ldc search
ldl 5
sub
brz done1
ldc search
ldl 6
sub
brz done2
ldc search
ldl 7
sub
brz done3
br done4
done1: ldc 5
HALT
done2: ldc 6
HALT
done3: ldc 7
HALT
done4: ldc -1
HALT