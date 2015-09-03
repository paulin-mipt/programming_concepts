CMD_DICT = {
    'TERM': 'FF', # terminate
    'ASG': '0', # assign
    'INP': '1', # read integer to variable
    'OUT': '2', # print integer to stdout
    'ADD': '3', # A := A + B
    'SUB': '4', # A := A - B
    'JMPIFLEQ': '5', # 'JMPIFLEQ A B C' means 'if A <= B: jump to C'
    'DUMP': '6' # Memory dump to output
}

ARG_NUM_DICT = {
    'TERM': 0,
    'ASG': 2,
    'INP': 1,
    'OUT': 1,
    'ADD': 2,
    'SUB': 2,
    'JMPIFLEQ': 3,
    'DUMP': 0
}

RESERVED_REGS_NUM = 1
INT_SIZE = 4


class AsmException(Exception):
    pass


def get_bytes(dec_num, size):
    return int(dec_num).to_bytes(size, byteorder='big', signed=False)

addr_dict = {'IP': 0} # IP - number of the first line containing a command
val_dict = {}

file_asm = open('asmcode', 'r')
lines = [line.split() for line in file_asm if len(line) > 0]

ip_line = lines[0]
if not (len(ip_line) == 4 and ip_line[0] == 'VAR' and ip_line[1] == 'IP' and ip_line[2] == '='):
    raise AsmException('The first line should be VAR IP = IP_VALUE')

val_dict['IP'] = int(ip_line[-1])
var_byte = [get_bytes(ip_line[-1], 4)]

last_addr_available = RESERVED_REGS_NUM
i = 1
while lines[i][0] == 'VAR':
    line = lines[i]
    if len(line) == 2:
        addr_dict[line[1]] = last_addr_available
        val_dict[line[1]] = 0
        var_byte.append(get_bytes(0, 4))
        last_addr_available += 1
    elif len(line) == 4 and line[2] == '=':
        addr_dict[line[1]] = last_addr_available
        if line[3] in val_dict.keys():
            var_byte.append(get_bytes(val_dict[line[3]], 4))
        else:
            var_byte.append(get_bytes(line[3], 4))
        val_dict[line[1]] = int(line[-1])
        last_addr_available += 1
    else:
        raise AsmException('line {}: usage: VAR VAR_NAME [= DECIMAL_VALUE]'.format(i))

    i += 1

bytes_out = var_byte

for line in lines[val_dict['IP'] - RESERVED_REGS_NUM + 1:]:
    cmd = line[0]
    bytes_out.append(get_bytes(int(CMD_DICT[cmd], base=16), 1))
    for i in range(ARG_NUM_DICT[cmd]):
        bytes_out.append(get_bytes(addr_dict[line[i + 1]], 1))
    for i in range(3 - ARG_NUM_DICT[cmd]):
        bytes_out.append(get_bytes(0, 1))

with open('bytecode', 'wb') as file_byte:
    file_byte.write(bytearray().join(bytes_out))
