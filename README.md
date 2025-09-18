# Gömülü Sistemler Laboratuvar Ödevleri

Bu veri havuzu, **Gömülü Sistemler** dersi kapsamında tamamlanan laboratuvar ödevini içerir. Laboratuvarlar, Polonya, Kraków’daki **AGH Bilim ve Teknoloji Üniversitesi**’nde Erasmus+ bursu aldığım dönemde müfredatımın bir parçasıydı.

---

## 🚀 Proje Genel Bakışı

Bu ödevler, temel gömülü sistem kavramlarını uygulamalı olarak öğrenmeye odaklanır. Öne çıkan başlıklar şunlardır:

- **GPIO Kontrolü**: LED'ler gibi harici bileşenleri kontrol etmek için Genel Amaçlı Giriş/Çıkış pinlerini yönetme.  
- **Çevresel İletişim**: UART (Evrensel Asenkron Alıcı/Verici) protokolünü kullanarak bir bilgisayar terminali ile veri alışverişi yapma.  
- **Kesintiler**: Donanım kesintilerini kullanarak duyarlı, olay odaklı davranışlar oluşturma.  

Tüm kodlar, **FRDM-KL05Z geliştirme kartı** için **C dilinde** yazılmıştır.

---

## 📁 Dosya Açıklamaları

| Dosya | Açıklama |
|-------|----------|
| `main.c`, `main.h` | Ana uygulama giriş noktası ve genel tanımlar. UART komutlarını işleme ve LED kontrolü için temel mantık burada uygulanır. |
| `led.c`, `led.h` | LED kontrol modülü. LED başlatma, açma/kapama ve basit karşılama dizisi işlevlerini içerir. |
| `uart0.c`, `uart0.h` | UART0 çevre birimi üzerinden iletişimi yönetir. Düşük seviyeli kayıt yapılandırması ve veri almak için kesme hizmet rutinlerini içerir. |
| `frdm_bsp.h` | FRDM-KL05Z kartına özgü pin maskeleri ve gecikme fonksiyonları gibi faydalı makrolar içerir. |

---

## 🛠️ Kurulum ve Kullanım

Bu kodlar, kursta kullanılan **donanım ve geliştirme ortamı** için hazırlanmıştır. Derlemek ve çalıştırmak için şunlara ihtiyacınız olacak:

1. **ARM GCC araç zinciri** (Gömülü geliştirme için)  
2. **Keil uVision** veya KL05Z mikrodenetleyicisini destekleyen benzer bir IDE  
3. **FRDM-KL05Z Freedom geliştirme kartı**

---

## 💡 Öğrenme Yansımaları

Bu ders, düşük seviyeli gömülü programlama konusunda değerli uygulamalı deneyimler kazanmamı sağladı. Mikrodenetleyici mimarisi ve gerçek zamanlı sistemler hakkında anlayışımı derinleştirdi.  

Ayrıca, **yurt dışında eğitim alma** ve yeni bir teknik ortama uyum sağlama deneyimi, akademik ve kişisel gelişimimin önemli bir parçası oldu.

---

> Made with ❤️ during my Erasmus+ internship at AGH University of Science and Technology, Kraków, Poland.
