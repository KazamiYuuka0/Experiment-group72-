from gmssl import sm3
def hex2bytes(hexnum):
    hexbyte = hexnum.to_bytes((hexnum.bit_length() + 7) // 8, byteorder='big')
    #print(hex(hexnum))
    #print(hexbyte)
    #print()
    return hexbyte
def bytes2hex(hexbyte):
    hexnum=int.from_bytes(hexbyte, byteorder='big')
    #print(hexbyte)
    #print(hex(hexnum))
    #print()
    return hexnum
#杂凑算法选择SM3-256
def Hash_sm3_b2b(message):
    msg_list = [i for i in message]
    hash_hexstr = sm3.sm3_hash(msg_list)
    hash_bytes = bytes.fromhex(hash_hexstr)
    return hash_bytes
def Hash_sm3_b2i(message):
    msg_list = [i for i in message]
    hash_hexstr = sm3.sm3_hash(msg_list)
    hash_bytes = bytes.fromhex(hash_hexstr)
    return bytes2hex(hash_bytes)
#椭圆曲线点转换#采用未压缩形式
def dot2bytes(x,y):
    yb=hex2bytes(y)
    l=len(yb)
    if l<32:#y扩充到32B
        yb=bytes([0x0]*(32-l))+yb
    return bytes([0x04])+hex2bytes(x)+yb
def bytes2dot(dotbyte):
    PC=bytes2hex(dotbyte[:1])
    y1=bytes2hex(dotbyte[-32:])
    x1=bytes2hex(dotbyte[1:-32])
    if PC!=0x04:
        return -1,-1
    else:
        return x1,y1
    
