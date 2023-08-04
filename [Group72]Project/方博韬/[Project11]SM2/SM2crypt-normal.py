from basic_func import hex2bytes,bytes2hex,Hash_sm3_b2b,Hash_sm3_b2i,dot2bytes,bytes2dot
import random
from EC_func import mod_inv,field_add,field_PadP,field_mul_optimized
#SM2-256
#此代码实现存在一定问题，只接受小于256比特的明文输入
h=1#SM2固定参数，表示椭圆曲线群中的所有点都属于主要子群
p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
xG=0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D
yG=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2
n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7
M='encryption standard'
dB=0x1649AB77A00637BD5E2EFE283FBF353534AA7F7CB89463F208DDBC2920BB0DA0
#d1=random.randint(1,n-1)
#d2=random.randint(1,n-1)
#dB=pow(d1*d2,-1,n)
#print('d1:',hex(d1))
#print('d2:',hex(d2))
#print('dB:',hex(dB))
#d1=0x3bd2d49498769bfd5db118a36325366962d5af712833afd717878aa78fc5cd8c
#d2=0x84c2f88319c7d6e5dc9094afe891e85acf5197c7d94db720e7af3e6566fc84ab
#dB=pow(d1*d2,-1,n)
#
xPB,yPB=field_mul_optimized(a,p,dB,xG,yG)#B的公钥

def KDF(Z):#klen=256bit#返回int
    ct=bytes([0x00000001])
    #因为klen=v=256，略去循环
    K=Hash_sm3_b2i(Z+ct)
    return K
def sm2_enc(a,p,n,h,M,xPB,yPB,xG,yG):
    k=random.randint(1,n-1)
    x1,y1=field_mul_optimized(a,p,k,xG,yG)
    C1=dot2bytes(x1,y1)#8+256+256
    xS,yS=field_mul_optimized(a,p,h,xPB,yPB)
    if xS==-1  and yS==-1:
        return 0
    x2,y2=field_mul_optimized(a,p,k,xPB,yPB)
    t=KDF(hex2bytes(x2)+hex2bytes(y2))
    while t==0x0:
         k=random.randint(1,n-1)
         x1,y1=field_mul_optimized(a,p,k,xG,yG)
         C1=dot2bytes(x1,y1)
         x2,y2=field_mul_optimized(a,p,k,xPB,yPB)
         t=KDF(hex2bytes(x2)+hex2bytes(y2))
    #
    M=M.encode('ascii')
    C2=hex2bytes(bytes2hex(M)^t)#256
    C3=Hash_sm3_b2b(hex2bytes(x2)+M+hex2bytes(y2))#256
    return C1+C2+C3
def sm2_dec(a,p,n,h,b,dB,C):
    #klen=256
    C1=C[:-64]
    C2=C[-64:-32]
    C3=C[-32:]
    print(C1)
    x1,y1=bytes2dot(C1)
    if (y1**2)%p!=(x1**3+a*x1+b)%p:#注意此处模p
        return -1
    else:
        xS,yS=field_mul_optimized(a,p,h,x1,y1)
        x2,y2=field_mul_optimized(a,p,dB,x1,y1)
        t=KDF(hex2bytes(x2)+hex2bytes(y2))
        M=bytes2hex(C2)^t
        u=Hash_sm3_b2b(hex2bytes(x2)+hex2bytes(M)+hex2bytes(y2))
        if u!=C3:
            return -1
        else:
            #print(hex(M))
            return M
print(hex(sm2_dec(a,p,n,h,b,dB,sm2_enc(a,p,n,h,M,xPB,yPB,xG,yG))))
#print('C:',sm2_enc(a,p,n,h,M,xPB,yPB,xG,yG))

