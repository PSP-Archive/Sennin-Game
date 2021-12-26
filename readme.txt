readme.txt
japanese(s-jis) text format

---------------------------------------------------
sennin game   /  仙人ゲーム

PSP (firmware 1.00) 用の 仙人ゲーム
[firmware1.5での動作検証はしてません。]

遊び方・メモリーステック内に

	PSP
	 GAME
	  SENNIN
	   EBOOT.PBP

このように配置してください。あとはメモリステックから起動
終了は[HOME]キーです。

アナログステックで、善仙人を操って悪仙人を避けてください。
それだけ。基本的によけるだけです。

はるか古、X68000というマシンで稼動していたゲームです。

---------------------------------------------------
v0.01	[Jun30,2005]

こっそりuploadしてみたり。製作期間4日。

えっと、PSPのテストで作ったもので、まだぜんぜん出来てないです。
判定もかなりアバウトですし、敵も一定速度でしか動かないので
ぜんぜん面白くないです。
元データが見つからなかったので、10年以上前の仙人のキャラ絵を
なんとなく思い出しながら描く。なんかぜんぜん違うものができた。
久々に、ドット打った（笑）

今回はEBOOTだけのリリースですが、もうちょっといじったら
ソースも公開しようと思っております。

グラフィックデータコンバートはぶーにゃんさんのpng->c コンバータを
利用させていただいてます。
http://bu-nyan.m.to/


p.s. ゲーム中、×ボタンでなつかしの背景スクロールがっ！(力技)

---------------------------------------------------
v0.02	[Jul 2,2005]

サウンドの追加(nemさんのコードを参考に）
曲データはnemさんのサンプルコードに入っていたものを
そのまま利用。
サウンドコードは再利用しやすいようにいじったつもりだけど、
どんなもんでしょう。

bg0.wav/se0.wavをEBOOT.PBPと同じところに入れてください

	PSP
	 GAME
	  SENNIN
	   EBOOT.PBP
       bg0.wav
       se0.wav



[コメント]
v0.01は公開3時間で http://psp.holybell.to/ に ばれてるし・・・

ダウン数も結構行ったけど、なんか何の反応も無いし、
もうちょっとゲームとしてブラッシュアップしようと思ったけど
なんだかやる気無くなったので、そのままソースアップ。


コードは再利用してもらってかまいませんが、
'website MAGiCTOUCH' http://www10.big.or.jp/~and/
を入れておいてもらえるとうれしい（強制じゃないです）。

---------------------------------------------------
Special thanks to...

http://psp.holybell.to/
http://pspdev.ofcode.com/api.php

http://mypage.odn.ne.jp/home/nataka
 (開発環境の構築はこちらを参考にさせていただきました。）

  and all homebrew PSP Developer.


PSPプログラムに関しては先人の方々の功績により、かなり楽に
動くものが作れました。ありがとうございます。
ソース公開されている方のプログラムからいろいろと切った貼った
させていただきました。
---------------------------------------------------
SEC - Saturn Expedition Committee		http://sec.pn.to/
Music/Sound matherial by ASOBEAT		http://www.yonao.com/asobeat/
Sound matherial by TAM Music Factory	http://www.tam-music.com/
website MAGiC TOUCH						http://www10.big.or.jp/~and/
