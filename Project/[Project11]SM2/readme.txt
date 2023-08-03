SM2数字签名实现包括：normal原版 & RFC6979版
RFC6979版改变了k的生成方式，不再采用随机值。使用HMAC多伦运算对给定输入生成固定的k。
具体调用在kgen中。

数字签名主体文件SM2sign-normal&SM2sign-RFC6979最后两个函数sm2_sign、sm2_verf分别为SM2签名和验证函数。
椭圆曲线参数和签名参数全局变量可以删除，调用数字签名函数时导入需要的参数即可。

basic_func.py  字符转换和SM3杂凑函数
EC_func.py     椭圆曲线运算
ECC.py         ECC加密算法
kgen.py        RFC6979的k生成



