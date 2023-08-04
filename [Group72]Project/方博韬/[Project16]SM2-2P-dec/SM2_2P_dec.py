from basic_func import hex2bytes,bytes2hex,Hash_sm3_b2b,Hash_sm3_b2i,dot2bytes,bytes2dot
import random
from EC_func import mod_inv,field_add,field_PadP,field_mul_optimized,field_sub
h=1#SM2固定参数，表示椭圆曲线群中的所有点都属于主要子群
p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
xG=0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D
yG=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2
n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7

d1=0x3bd2d49498769bfd5db118a36325366962d5af712833afd717878aa78fc5cd8c
d2=0x84c2f88319c7d6e5dc9094afe891e85acf5197c7d94db720e7af3e6566fc84ab
xP,yP=field_mul_optimized(a,p,mod_inv(d1*d2,n),xG,yG)#B的公钥
C=b"\x04\x0e\xbd\x02N\x13\xf6\xbd\xcb~\x8c\x9b\x9b\x14_\xce[\xbdE;H80\xc8\xbd-\x8dVcp\xc1\x8e\xff;\x9c\xf5r\xa3.\x9c\x1d\xef\xe1\x02\xf7\x17\x00\xdaj\x86#W\xa4\xa5p8\xb8\xf4G\x94+\x81\x081\x92b\xb0L\xd4J\xcf\xc5\xc9\xa9\xf7I\xcej\x80\xe8Z\xd3\x9a\xcc5\xb6\r\xa0{\x84\xe63w\xa1\xca7\x8b/\xd2\t\xbd\xa0\x0e\x93e\x80\xbd~'F\xc7N]JI\xcc\x17\xe0Ga\xb3\xbd?\xc4Q\x88\xffy\xa3"
def KDF(Z):#klen=256bit#返回int
    ct=bytes([0x00000001])
    #因为klen=v=256，略去循环
    K=Hash_sm3_b2i(Z+ct)
    return K
def alice1(a,p,n,d1,C):
    #cipher C=C1||C2||C3
    C1=C[:-64]
    C2=C[-64:-32]
    C3=C[-32:]
    x1,y1=bytes2dot(C1)
    if x1==-1 and y1==-1:
        return -1
    xT1,yT1=field_mul_optimized(a,p,mod_inv(d1,n),x1,y1)
    T1=dot2bytes(xT1,yT1)
    return T1,C1,C2,C3,x1,y1
    #send(T1)
        

def bob1(a,p,n,d2,T1):
    #recv T1
    xT1,yT1=bytes2dot(T1)
    xT2,yT2=field_mul_optimized(a,p,mod_inv(d2,n),xT1,yT1)
    T2=dot2bytes(xT2,yT2)
    return T2
    #send(T2)
    

def alice2(a,p,n,d1,C2,C3,T2):
    #recv T2
    xT2,yT2=bytes2dot(T2)
    x2,y2=field_sub(a,p,xT2,yT2,x1,y1)#T2-C1
    t=KDF(hex2bytes(x2)+hex2bytes(y2))
    M_=bytes2hex(C2)^t
    u=Hash_sm3_b2b(hex2bytes(x2)+hex2bytes(M_)+hex2bytes(y2))
    if u==C3:
        return hex2bytes(M_)
    else:
        return 1
    
T1,C1,C2,C3,x1,y1=alice1(a,p,n,d1,C)
T2=bob1(a,p,n,d2,T1)
M_=alice2(a,p,n,d1,C2,C3,T2)

