def add_inv(P,p):#椭圆曲线加法逆元
    #tuple
    Q=list(P)
    Q[1]=-Q[1]%p
    return tuple(Q)
def mod_inv(m,p):#模p下的乘法逆元
    return pow(m,-1,p)
def add(a,p,P,Q=(-1,-1)):
    x1=P[0]
    y1=P[1]
    x2=Q[0]
    y2=Q[1]
    if P==(-1,-1)and Q==(-1,-1):
        return (-1,-1)
    elif P==(-1,-1)and Q!=(-1,-1):
        return Q
    elif Q==(-1,-1)and P!=(-1,-1):
        return P
    elif P==Q:
        k=((3*x1*x1+a)*mod_inv(2*y1,p))%p
    else:
        k=((y2-y1)*mod_inv(x2-x1,p))%p
        
    x3=(k*k-x1-x2)%p
    y3=((k*x1)-(k*x3)-y1)%p
    return (x3,y3)
def n_add(a,p,P,n=0):
    temp=P
    for i in range(n-1):
        temp=add(a,p,temp,P)
    return temp
    
p=29
a=4
b=20
k=25
G=(13,23)

K=n_add(a,p,G,k)
print(K)

r=6
M=(3,28)
C1=add(a,p,M,n_add(a,p,K,r))
print(C1)
C2=n_add(a,p,G,r)
print(C2)

Result=add(a,p,C1,add_inv(n_add(a,p,C2,k),p))
print(Result)

   
        
