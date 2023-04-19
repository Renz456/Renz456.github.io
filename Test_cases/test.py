import random
import math

S = 256 # 16 bits for set
E = 4 # number of lines per set
B = 256 # 16 bits for bytes per block
m = 32 # this means we have 16 bits for tag

s = math.log2(S)
b = math.log2(B)
t = m - (s+b)

T = 2**t

num_cache = 4 #needs to be changed here

caches = list(range(1, num_cache+1))
random.seed()
num_trace = 100000

for numbers in caches:
    collect = {}
    for i in range(num_trace):
        collect[i] = []
        #generate r or w
        r_or_w = random.randint(0, 1)
        if r_or_w:
            collect[i].append("r")
        else:
            collect[i].append("w")
        #generate the address
        set_num = random.randint(0, S)
        block_num = random.randint(0, B)
        tag_num = random.randint(0, T)

        address = hex(tag_num)
        set_num_hex = hex(set_num)
        block_num_hex = hex(block_num)
        set_num_hex = '0x' + set_num_hex[2:].zfill(int(s/4))
        block_num_hex = '0x' + block_num_hex[2:].zfill(int(b/4))
        address = '0x' + address[2:].zfill(int(t/4))
        #print(type(address), set_num_hex[2:len(set_num_hex)], type(block_num_hex))
        address = address + set_num_hex[2:len(set_num_hex)] + block_num_hex[2:len(block_num_hex)]
        collect[i].append(address)
        collect[i].append(random.randint(0, (B-block_num)))

    file_name = 'ocean' + str(numbers) + '.trace'
    with open(file_name, 'w') as f:
        for i in range(num_trace):
            f.write(collect[i][0])
            f.write(", ")
            f.write(collect[i][1])
            f.write(", ")
            f.write(str(collect[i][2]))
            f.write("\n")
