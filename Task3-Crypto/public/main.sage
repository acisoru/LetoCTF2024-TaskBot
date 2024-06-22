from secrets import ec_params, a_priv, flag
from Crypto.Cipher import AES
from hashlib import sha256
from Crypto.Util.Padding import pad
from Crypto.Util.number import long_to_bytes
from os import urandom

Gx, Gy =926644437000604217447316655857202297402572559368538978912888106419470011487878351667380679323664062362524967242819810112524880301882054682462685841995367, 4856802955780604241403155772782614224057462426619061437325274365157616489963087648882578621484232159439344263863246191729458550632500259702851115715803253

p, a, b = ec_params

R.<x> = PolynomialRing(GF(p))
f = x^2 + a * x + b
x1, x2 = f.roots(multiplicities=False)
print(f"x1= {x1}\nx2= {x2}")

E = EllipticCurve(GF(p), [a, b])
G = E(Gx, Gy)
secret = (G * a_priv).xy()[0]

hash = sha256()
hash.update(long_to_bytes(int(secret)))
key = hash.digest()[16:32]
iv = urandom(16)
cipher = AES.new(key, AES.MODE_CBC, iv)

encrypt = cipher.encrypt(pad(flag, 16))
print(f"encrypt={encrypt}")
print(f"iv={iv}")
