# Pretty Good Privacy

<div align="center">

[![C](https://img.shields.io/badge/C-12.0.5-orange)](https://devdocs.io/c/)
[![RSA](https://img.shields.io/badge/RSA-AsymmetricEncode-blue)](https://ko.wikipedia.org/wiki/RSA_%EC%95%94%ED%98%B8)
[![MD5](https://img.shields.io/badge/MD-5-lightgrey)](https://ko.wikipedia.org/wiki/MD5)
[![S-DES](https://img.shields.io/badge/DES-SymmetricEncode-blueviolet)](https://ko.wikipedia.org/wiki/%EB%8D%B0%EC%9D%B4%ED%84%B0_%EC%95%94%ED%98%B8%ED%99%94_%ED%91%9C%EC%A4%80)
[![MIT](https://img.shields.io/badge/License-MIT-blueviolet)](https://ko.wikipedia.org/wiki/MIT_%ED%97%88%EA%B0%80%EC%84%9C)

</div>

<div align="center">
  <img width="50%"  src="https://cdn.worldvectorlogo.com/logos/pgp-security.svg" />
</div>
  


### 구현 환경

- Platform: MAC
- IDE: CLion
- Language: C

### 구현 정도

- [x] RSA key 생성
- [x] Digest 생성(MD5 hasing)
- [x] MAC 생성 (RSA로 서명)
- [ ] 압축
- [x] Session key 생성 (Random 생성)
- [X] Message 암호화 (S-DES)
- [x] Session key 암호화 (RSA로 암호화)
- [ ] BASE64 encoding 

### 소개

- gen-rsa-key: RSA key를 생성합니다.
  ```bash
  ./gen-rsa-key [prime Number] [prime Number] [생성 파일 위치]
  # example
  ./gen-rsa-key 17 13 keyring/sender.txt
  ```
- pgp-sender: PGP를 이용하여 Message를 암호화합니다.
  ```bash
  ./pgp-sender [전송할 파일] [생성된 파일]
  # example
  ./pgp-sender Text.txt result/EText.txt
  ```
- pgp-receiver: PGP를 이용하여 Message를 복호화합니다.
  ```bash
  ./pgp-receiver [읽을 파일] [생성된 파일]
  # example
  ./pgp-receiver result/EText.txt result/DText.txt
  ```

### Run

```bash
echo "Pretty Good Privacy." >> Text.txt
make run
```

### PGP 소개

1. Authentication
   공개키 쌍이 하나 필요합니다.
    - 서명 (Sender)
        - message를 SHA-1, MD5 등을 통해 hashing합니다. = digest
        - 이를 sender의 private key를 통해 암호화합니다.  = digital signatures, finger print
          ⇒ RSA 알고리즘 사용
        - message와 만들어진 digital signature를 합칩니다.
    - 서명확인 (Receiver)
        - message와 digital signature를 분리합니다.
        - digital signature은 sender의 public key를 통해서 복호화합니다.
        - message에 sender와 동일한 hash function을 적용하고, 복호화된 digital signature와 동일한지 확인합니다.
2. Compression
    - 압축
        - 서명 단계에서 verification을 수행하기 때문에, 그 이후에 압축을 한다.
        - 암호화 시에는 데이터가 짧을 수록 적을 수록 좋기 때문에, 압축 이후에 암호화를 수행한다.
    - 압축 해제
        - 서명확인 이전에 압축을 해제해야 합니다.
3. Confidentiality
   공개키 쌍이 하나 필요합니다.
    - 암호화 (Sender)
        - session key를 생성합니다.
        - 압축을 통해 작아진 message를 session key를 통해서 암호화 합니다.

          ⇒ 56bit DES, 128bit CAST of IDEA, 168bit Triple DES 등을 활용합니다.

        - session key를 receiver의 public key를 통해서 암호화합니다.
          ⇒ RSA 알고리즘을 이용합니다.
        - 암호화된 message와 암호화된 session key를 합칩니다.
    - 복호화 (Receiver)
        - 암호화된 message와 암호화된 session key를 분리합니다.
        - receiver의 private key로 session key를 복호화합니다.
        - session key로 암호화된 message를 복호화합니다.
4. Compatibility (=호환성)
    - Encoding
        - PGP는 전송 시에 binary data 형태여야 하는데, email은 text만을 위해 설계되었다. 그렇기 때문에, PGP는 출력할 수 있는 ASCII 문자를 raw binary data로 encoding해야 한다.
        - 이때, radix-64 algorithm을 이용한다. (3 byte ⇒ 4char(= 4byte)) [8bit ⇒ 6bit]
          실제로 데이터는 3개짜리가 4개가 된다.
        - 기존에는 6 bit로 표현되던 데이터를 호환을 위해 8 bit 의 ASCII 데이터로 변경한다.
          
          Radix64 or Base64라고 불린다.

    - Decoding
        - 시스템에 알맞는 문자로 변환한다.
