import random
import math

S = 256 # 8 bits for set
E = 4 # number of lines per set
B = 65536 # 16 bits for block offset
m = 64 # this means we have 40 bits for tag

s = math.log2(S)
b = math.log2(B)
t = m - (s+b)

T = 2**t

num_cache = 8 #needs to be changed here

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

        address = bin(tag_num)[2:]
        set_num_hex = bin(set_num)[2:]
        block_num_hex = bin(block_num)[2:]
        set_num_hex = set_num_hex.zfill(int(s))
        block_num_hex = block_num_hex.zfill(int(b))
        address = address.zfill(int(t))
        address = address + set_num_hex + block_num_hex
        address = hex(int(address, 2))
        #print((address))
        collect[i].append(address)
        collect[i].append(random.randint(0, (B-block_num)))
        
            
def populate_same(collect, true_false):
    for i in range(num_trace):
        if(i == 0):
            collect[i] = []
            #generate r or w
            collect[i].append("w")
            #generate the address
            set_num = random.randint(0, S)
            block_num = random.randint(0, B)
            tag_num = random.randint(0, T)

            address = bin(tag_num)[2:]
            set_num_hex = bin(set_num)[2:]
            block_num_hex = bin(block_num)[2:]
            set_num_hex = set_num_hex.zfill(int(s))
            block_num_hex = block_num_hex.zfill(int(b))
            address = address.zfill(int(t))
            address = address + set_num_hex + block_num_hex
            address = hex(int(address, 2))
            #print((address))
            collect[i].append(address)
            collect[i].append(random.randint(0, (B-block_num)))
            #print(collect[i],"i printed here")
            #print(collect)
        else:
            collect[i] = []
            #print(collect[0], 'PRINTED HEREEEE')
            collect[i].append(collect[0][0])
            collect[i].append(collect[0][1])
            collect[i].append(collect[0][2])

def populate_arrayac(collect, address_of_1):
    if(address_of_1 == ""):
        for i in range(num_trace):
            if(i == 0):
                collect[i] = []
                #generate r or w
                collect[i].append("w")
                #generate the address
                set_num = random.randint(0, S)
                block_num = random.randint(0, B)
                tag_num = random.randint(0, T)

                address = bin(tag_num)[2:]
                set_num_hex = bin(set_num)[2:]
                block_num_hex = bin(block_num)[2:]
                set_num_hex = set_num_hex.zfill(int(s))
                block_num_hex = block_num_hex.zfill(int(b))
                address = address.zfill(int(t))
                address = address + set_num_hex + block_num_hex
                address = hex(int(address, 2))
                #print((address))
                collect[i].append(address)
                collect[i].append(random.randint(0, (B-block_num)))
                #print(collect[i],"i printed here")
                #print(collect)
            else:
                collect[i] = []
                #print(collect[0], 'PRINTED HEREEEE')
                collect[i].append(collect[0][0])
                address = int(collect[i-1][1])
                address += num_cache
                collect[i].append(hex(address))
                collect[i].append(collect[0][2])
    else:
        for i in range(num_trace):
            if(i == 0):
                collect[i] = []
                #generate r or w
                collect[i].append("w")
                address = int(address_of_1, 16) + 1
                collect[i].append(hex(address))
                collect[i].append(random.randint(0, (B)))
                #print(collect[i],"i printed here")
                #print(collect)
            else:
                collect[i] = []
                #print(collect[0], 'PRINTED HEREEEE')
                collect[i].append(collect[0][0])
                address = int(collect[i-1][1], 16)
                address += num_cache
                #print(address)
                collect[i].append(hex(address))
                collect[i].append(collect[0][2])


                
#To produce random traces
for numbers in caches:
    collect = {}
    populate(collect, True)
   # print(collect[0])
    file_name = 'rand' + str(numbers) + '.trace'
    with open(file_name, 'w') as f:
        for i in range(num_trace):
            f.write(collect[i][0])
            f.write(", ")
            f.write(collect[i][1])
            f.write(", ")
            f.write(str(collect[i][2]))
            f.write("\n")

#To produce consecutive same traces
for numbers in caches:
    collect = {}
    populate_same(collect, True)
    file_name = 'consec' + str(numbers) + '.trace'
    with open(file_name, 'w') as f:
        for i in range(num_trace):
            f.write(collect[i][0])
            f.write(", ")
            f.write(collect[i][1])
            f.write(", ")
            f.write(str(collect[i][2]))
            f.write("\n")

#To produce consec array access
for numbers in caches:
    if(numbers == 0):
        address_of_1 = ""
    else:
        address_of_1 = collect[0][1]
    print(address_of_1)
    collect = {}
    populate_arrayac(collect, address_of_1)
   # print(collect[0])
    file_name = 'arrayac' + str(numbers) + '.trace'
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

            address = bin(tag_num)[2:]
            set_num_hex = bin(set_num)[2:]
            block_num_hex = bin(block_num)[2:]
            set_num_hex = set_num_hex.zfill(int(s))
            block_num_hex = block_num_hex.zfill(int(b))
            address = address.zfill(int(t))
            address = address + set_num_hex + block_num_hex
            address = hex(int(address, 2))
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

            address = bin(tag_num)[2:]
            set_num_hex = bin(set_num)[2:]
            block_num_hex = bin(block_num)[2:]
            set_num_hex = set_num_hex.zfill(int(s))
            block_num_hex = block_num_hex.zfill(int(b))
            address = address.zfill(int(t))
            address = address + set_num_hex + block_num_hex
            address = hex(int(address, 2))
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
