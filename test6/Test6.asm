;multipication of 19*57
ldc 19
ldc 57
adc -1
loop: brz 5
stl 10
adc 19
ldl 10
adc -1
br loop
stnl 0
HALT