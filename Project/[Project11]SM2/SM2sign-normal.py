from gmssl import sm3
from basic_func import hex2bytes,bytes2hex,Hash_sm3_b2b,Hash_sm3_b2i
import random
from EC_func import mod_inv,field_add,field_PadP,field_mul_optimized
#默认参数GB/T 32918
def security_para():
    p = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
    a = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
    b = 0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
    n = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
    xG= 0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
    yG= 0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0
    G = (xG, yG)
    h = 1
    return p, a, b, n, G, h
#SM2数字签名测试参数(SM2文档附录参数)
p = 0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
a = 0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
b = 0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
xG = 0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D
yG = 0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2
n = 0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7
G = (xG, yG)
h = 1
ENTLA=0x0080
IDA ='ALICE123@YAHOO.COM'.encode('ascii')
M = 'message digest'
dA = 0x128B2FA8BD433C6C068C8D803DFF79792A519A55171B1B650C23661D15897263
xA = 0x0AE4C7798AA0F119471BEE11825BE46202BB79E2A5844495E97C04FF4DF2548A
yA = 0x7C0240F88F1CD4E16352A73C17B7F16F07353E53A176D684A9FE0C6BB798E857
PA = (xA, yA)
k = 0x6CB28D99385C175C94F94E934817663FC176D925DD72B727260DBAAE1FB2F96F
ZA=0xF4A38489E32B45B6F876E3AC2168CA392362DC8F23459C1D1146FC3DBFB7BC9A

#算法简单模拟
def sm2_pk_gen(a,p,dA,xG,yG):
    xP,yP=field_mul(a,p,dA,xG,yG)
    
def sm2_sign(ZA,M,n,a,p,xG,yG,dA):
    M_=ZA+M.encode('ascii')#A1
    e=Hash_sm3_b2i(M_)#A2
    k=random.randint(1,n-1)#A3
    x1,y1=field_mul_optimized(a,p,k,xG,yG)#A4
    r=(e+x1)%n#A5
    while r==0 or r+k==n:
        k=random.randint(1,n-1)
        x1,y1=field_mul_optimized(a,p,k,xG,yG)
        r=(e+x1)%n
    s=(mod_inv(1+dA,n)*(k-r*dA))%n#A6
    print('r:',r)
    print('s:',s)
    return r,s

def sm2_verf(r,s,ZA,M,n,a,p,xG,yG,xA,yA):
    if r<1 or r>n-1 or s<1 or s>n-1:#B1,B2
        print('failed')
        return 0
    M_=ZA+M.encode('ascii')#B3
    e=Hash_sm3_b2i(M_)#B4
    t=(r+s)%n#B5
    if t==0:
        print('failed')
        return 0
    xsG,ysG=field_mul_optimized(a,p,s,xG,yG)#B6
    xtP,ytP=field_mul_optimized(a,p,t,xA,yA)
    x1,y1=field_add(a,p,xsG,ysG,xtP,ytP)
    R=(e+x1)%n#B7
    if r==R:
        print('verfied')
        return 1
    else :
        print('failed')
        return 0
ZA=hex2bytes(ZA)
r,s=sm2_sign(ZA,M,n,a,p,xG,yG,dA)
sm2_verf(r,s,ZA,M,n,a,p,xG,yG,xA,yA)

