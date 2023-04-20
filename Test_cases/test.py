import random
import math

S = 256 # 16 bits for set
E = 4 # number of lines per set
B = 256 # 16 bits for block offset
m = 32 # this means we have 16 bits for tag

s = math.log2(S)
b = math.log2(B)
t = m - (s+b)

T = 2**t

num_cache = 4 #needs to be changed here

caches = list(range(1, num_cache+1))
random.seed()
num_trace = 100000

def populate(collect, true_false):
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
            

#To produce random traces
for numbers in caches:
    collect = {}
    populate(collect, True)
    file_name = 'rand' + str(numbers) + '.trace'
    with open(file_name, 'w') as f:
        for i in range(num_trace):
            f.write(collect[i][0])
            f.write(", ")
            f.write(collect[i][1])
            f.write(", ")
            f.write(str(collect[i][2]))
            f.write("\n")

#Produce cases for true sharing, different caches want to read/write to 
#same address
for numbers in caches:
    if(numbers == 1):
        collect = {}
        populate(collect, True)
        file_name = 'true_share' + str(numbers) + '.trace'
        with open(file_name, 'w') as f:
            for i in range(num_trace):
                f.write(collect[i][0])
                f.write(", ")
                f.write(collect[i][1])
                f.write(", ")
                f.write(str(collect[i][2]))
                f.write("\n")
    else:
        file_name = 'true_share' + str(numbers) + '.trace'
        with open(file_name, 'w') as f:
            for i in range(num_trace):
                if(collect[i][0] == 'r'):
                    f.write("w")
                else:
                    f.write("r")
                f.write(", ")
                f.write(collect[i][1])
                f.write(", ")
                f.write(str(collect[i][2]))
                f.write("\n")

#Produce cases for false sharing, different caches want to read/write to 
#same cache block, but different address
for numbers in caches:
    if(numbers == 1):
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
            collect[i].append(set_num)
            collect[i].append(block_num)
            collect[i].append(tag_num)

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

        file_name = 'false_sharing' + str(numbers) + '.trace'
        with open(file_name, 'w') as f:
            for i in range(num_trace):
                f.write(collect[i][0])
                f.write(", ")
                f.write(collect[i][4])
                f.write(", ")
                f.write(str(collect[i][5]))
                f.write("\n")
    else:
        collect_new = {}
        for i in range(num_trace):
            collect_new[i] = []
            #generate r or w
            if(collect[i][0] == 'r'):
                collect_new[i].append('w')
            else:
                collect_new[i].append('r')
            #generate the address
            set_num = collect[i][1]
            block_num = collect[i][2]
            tag_num = collect[i][3]

            block_num = random.randint(block_num, B)

            address = hex(tag_num)
            set_num_hex = hex(set_num)
            block_num_hex = hex(block_num)
            set_num_hex = '0x' + set_num_hex[2:].zfill(int(s/4))
            block_num_hex = '0x' + block_num_hex[2:].zfill(int(b/4))
            address = '0x' + address[2:].zfill(int(t/4))
            #print(type(address), set_num_hex[2:len(set_num_hex)], type(block_num_hex))
            address = address + set_num_hex[2:len(set_num_hex)] + block_num_hex[2:len(block_num_hex)]
            collect_new[i].append(address)
            collect_new[i].append(random.randint(0, (B-block_num)))

        file_name = 'false_sharing' + str(numbers) + '.trace'
        with open(file_name, 'w') as f:
            for i in range(num_trace):
                f.write(collect_new[i][0])
                f.write(", ")
                f.write(collect_new[i][1])
                f.write(", ")
                f.write(str(collect_new[i][2]))
                f.write("\n")
