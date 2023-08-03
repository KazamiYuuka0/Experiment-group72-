from basic_func import hex2bytes,bytes2hex,Hash_sm3_b2b,Hash_sm3_b2i
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
#
ZA=0xF4A38489E32B45B6F876E3AC2168CA392362DC8F23459C1D1146FC3DBFB7BC9A
def HMAC(K,text):
    ipad=bytes2hex(bytes([0x36]*32))
    opad=bytes2hex(bytes([0x5c]*32))
    #
    klen=len(K)#字节数
    K=K+bytes([0x00]*(32-klen))#1
    k=bytes2hex(K)#2
    text=hex2bytes(k ^ ipad)+text#2&3
    text=Hash_sm3_b2b(text)#4
    k=k^opad#5
    kb=hex2bytes(k)+text#6
    return Hash_sm3_b2b(kb)#7  

def genK(M,dA,n):#3int
    M=M.encode('ascii')
    h1=Hash_sm3_b2b(M)
    V=bytes([0x01]*32)
    K=bytes([0x00]*32)
    K=HMAC(K,V+bytes([0x00])+hex2bytes(dA)+h1)#长度不确定
    V=HMAC(K,V)
    K=HMAC(K,V+bytes([0x01])+hex2bytes(dA)+h1)
    V=HMAC(K,V)# 
    T=b''
    while len(T)<32:
        V=HMAC(K,V)
        T=T+V
    k=bytes2hex(T)  
    while k<1 or k>n:#SM2中n就是q
        K=HMAC(K,V+bytes([0x00]))
        V=HMAC(K,V)
        T=b''
        while len(T)<32:
            V=HMAC(K,V)
            T=T+V
        k=bytes2hex(T)
    return k
        
#print(hex(genK(M,dA,n))) 
    
