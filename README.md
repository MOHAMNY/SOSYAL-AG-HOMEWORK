# Sosyal Ağ Grafiği Analizi

Bu proje, bir sosyal ağ grafiğini ağaç tabanlı veri yapıları kullanarak analiz etmeyi amaçlamaktadır. Kullanıcılar ve arkadaşlık ilişkileri bir graf yapısı olarak modellenmiş, analiz işlemleri için DFS, topluluk tespiti, etki alanı analizi ve kırmızı-siyah ağaç (Red-Black Tree) gibi algoritmalar kullanılmıştır.

## Özellikler

- Kullanıcılar ve arkadaşlık ilişkileri veriseti.txt dosyasından okunur.
- İlişki grafı oluşturulur.
- Belirli mesafedeki arkadaşlar *derinlik öncelikli arama (DFS)* ile bulunur.
- İki kullanıcı arasındaki *ortak arkadaşlar* tespit edilir.
- Bağlantılı bileşenler üzerinden *topluluklar* belirlenir.
- Her kullanıcının *etki alanı* hesaplanır.
- Kullanıcı kimliklerine göre *Red-Black Tree* ile dengeli arama yapısı oluşturulur.

## Derleme ve Çalıştırma

```bash
gcc main.c -o sosyal_ag
./sosyal_ag
