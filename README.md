# Skywatch

# 前置說明
執行環境：Winsows
使用語言：C/C++

code資料夾存放Server、Client1、Client2、Client3的程式碼。

Server.exe為Server的執行檔。
Client1.exe為Client1的執行檔。
Client2.exe為Client2的執行檔。
Client3.exe為Client3的執行檔。
run.bat為批次檔，會一次執行Server.exe、Client1.exe、Clinet2.exe、Client3.exe這四種檔案。

# 執行方法
第一種方法：直接執行run.bat檔案(必須把run.bat、Server.exe、Client1.exe、Client2.exe、Client3.exe都放在同一個資料夾)。
第二種方法：對Server.exe、Client1.exe、Client2.exe、Client3.exe這四個執行檔，自行點擊開啟。

# 執行流程
開啟Server.exe後會出現一段文字，然後就可以輸入數字。數字要以空白隔開，最後要按下Enter。
之後Server就會把全部數字依序傳送給Client1、Client2、Client3。

Client1利用socket溝通，然後計算mean。計算完之後的結果會顯示在Client1的視窗上面。最後會把結果傳回到Server上。
Client2利用pipe溝通，然後計算median。計算完之後的結果會顯示在Client2的視窗上面。最後會把結果傳回到Server上。
Client3利用shared memory溝通，然後計算mode。計算完之後的結果會顯示在Client3的視窗上面。最後會把結果傳回到Server上。

最後Server把Client1、Clinet2、Client3傳回來的結果，依序顯示到Server的視窗上面。
