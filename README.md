# np_hw1

Network program homework 1

程式作業1

請分別使用fork與select撰寫web server程式。這個程式原則上需在port 80 listen，但若缺乏root權限的同學，也可以在其他port listen。除了程式本身外，請自行撰寫一個網頁，其中至少包含一張圖片，供作業demo之用。

1. web server需要能夠至少由一個主流瀏覽器(Chrome, Firefox, Edge...)連線並觀看網頁。
2. fork版本需要能正確避免zombie。
3. 可自行設計作業規格以外的功能，並在demo中描述，做為額外加分項目。

作業原始程式碼請上傳自行申請的github帳號。在demo作業時請現場clone並compile(可遠端到工作站、自己的電腦或自行帶筆電)。程式碼並請tar+gz一份上傳到ecourse備查。

server參考來源:

http://man7.org/linux/man-pages/man3/getaddrinfo.3.html

http://beej-zhtw.netdpi.net/05-system-call-or-bust/5-7-send

圖片讀取問題解決方法來源:

https://stackoverflow.com/questions/45670369/c-web-server-image-not-showing-up-on-browser

首先使用了getaddrinfo這個function, 接著讀res裡面的資料，作socket根bind,成功後,free掉res。

接著listen，可以的連線數量爲10

最後就是等着別人來連線,去做accept,接著呼叫fork來讓chil做事是，來看GET了什麼東西,依照條件來作處理,分別爲圖片及網頁處理,然後就結束