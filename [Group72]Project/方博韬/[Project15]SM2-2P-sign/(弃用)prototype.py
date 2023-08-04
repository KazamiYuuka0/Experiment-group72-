import random
from SM2sign_RFC6979 import sm2_verf
from basic_func import hex2bytes,bytes2hex,Hash_sm3_b2b,Hash_sm3_b2i,dot2bytes,bytes2dot
from EC_func import mod_inv,field_add,field_PadP,field_mul_optimized

p = 0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
a = 0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
b = 0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
xG = 0x421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D
yG = 0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2
n = 0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7
h = 1
M = 'message digest'
ZA= 'Identity of A&B'

#

def alice1(a,p,n,xG,yG):#公钥生成
    d1=random.randint(1,n-1)
    xP1,yP1=field_mul_optimized(a,p,mod_inv(d1,n),xG,yG)
    #send(xP1,yP1)
    return xP1,yP1,d1

def bob1(a,p,n,xG,yG):#公钥生成
    d2=random.randint(1,n-1)
    xP,yP=field_mul_optimized(a,p,mod_inv(d2,n),xG,yG)
    #public(xP,yP)
    return xP,yP,d2
    
def alice2(a,p,n,xG,yG,ZA,M):#秘密传输
    M_=ZA.encode('ascii')+M.encode('ascii')
    e=Hash_sm3_b2b(M_)#32byte

    k1=random.randint(1,n-1)
    xQ1,yQ1=field_mul_optimized(a,p,k1,xG,yG)
    text=dot2bytes(xQ1,yQ1)+e#Q1&e
    #send(xQ1,yQ1,e)
    return text,k1

def bob2(a,p,n,xG,yG,text,d2):#秘密传输
    e=bytes2hex(text[-32:])
    xQ1,yQ1=bytes2dot(text[:-32])
    k2=random.randint(1,n-1)
    k3=random.randint(1,n-1)
    xQ2,yQ2=field_mul_optimized(a,p,k2,xG,yG)
    x1,y1=field_add(a,p,*field_mul_optimized(a,p,k3,xQ1,yQ1),xQ2,yQ2)
    r=(x1+e)%n
    s2=(d2*k3)%n
    s3=d2*(r+k2)%n
    #加密(r,s2,s3)
    rb=hex2bytes(r)
    s2b=hex2bytes(s2)
    s3b=hex2bytes(s3)
    rl=len(rb)
    s2l=len(s2b)
    s3l=len(s3b)
    if rl<32:
        rb=bytes([0x0]*(32-rl))+rb
    if s2l<32:
        s2b=bytes([0x0]*(32-s2l))+s2b
    if s3l<32:
        s3b=bytes([0x0]*(32-s3l))+s3b
    text=rb+s2b+s3b
    #send(r,s2,s3)
    return text

def alice3(a,p,n,k1,text,d1):#获得签名
    r=bytes2hex(text[0:32])
    s2=bytes2hex(text[32:64])
    s3=bytes2hex(text[64:96])
    s=((d1*k1)%n*s2+d1*s3+mod_inv(r,n))%n
    if s!=0 and s!=n-r:
        return r,s
    else:
        return -1,-1


xP1,yP1,d1=alice1(a,p,n,xG,yG)
xP,yP,d2=bob1(a,p,n,xG,yG)
textA2B,k1=alice2(a,p,n,xG,yG,ZA,M)
textB2A=bob2(a,p,n,xG,yG,textA2B,d2)
r,s=alice3(a,p,n,k1,textB2A,d1)
print('r:',hex(r))
print('s:',hex(s))



