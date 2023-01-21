# MDXVV.X (開発中)
MDX Voice Viewer for X680x0/Human68k

MDXセレクタ＆プレーヤー＆音色データビュワー＆音色データ抽出ソフトです。

注意：現在開発中につきまだバイナリ配布パッケージは作っていません。この説明書きだけになります。

![](images/mdxvv1.png)

---

## インストール / Install

MDXVVxxx.ZIP をダウンロードして展開し、MDXVV.X をパスの通ったディレクトリにコピーします。

また、以下の実行ファイルにパスを通しておく必要があります。

* COMMAND.X
* MXP.X
* MMDSP.r (オプション)

MXDRV の常駐は必須です。

MXP.X (標準MDX再生コマンド) は MXDRV の本体パッケージに含まれています。X68000 LIBRARY から入手してください。
http://retropc.net/x68000/software/sound/mxdrv/mxdrv16/

ただし、MXDRV ドライバ本体は上記のものではなく 68030 対応版 (MXDRV30) の利用をお勧めします。こちらも X68000 LIBRARY から入手できます。
http://retropc.net/x68000/software/sound/mxdrv/mxdrv30/

起動ドライブのルートディレクトリに置かれた COMMAND.X へパスが通ってない場合がよくありますので注意してください。

---

## 起動方法 / How To Start

    MDXVV [options] [mdx-directory-name]

MDXファイルが格納されているフォルダを指定して起動します。
指定が無い場合はカレントディレクトリが指定されたものとして扱います。

オプションとして以下の指定が可能です。

・`-g`

グラフィック画面の初期化を行いません。壁紙画像を維持したい場合などに。MDXVV自体はテキスト0,1プレーンのみを使用します。

・`-u`

060turbo / PhantomX などのハイメモリをバッファとして使用します。使える状況にある場合はできる限り指定してください。

・`-f<n>`

自動演奏モードの際、n (秒数) で指定された時間が経過すると曲をフェードアウトして次に進みます。

・`-a`

自動演奏モードをOFFにして起動します。起動後も `a` キーで ON/OFF を切り替えることができます。


---

## 操作方法 / Key Bindings

操作はすべてキーボードで行います。

- `h`

主要な操作に関するヘルプメッセージを表示します。

- `ESC`
- `q`

MDXVVを終了します。

- `CR`
- `ENTER`
- `CTRL+m`

カーソルバーの位置のMDXを再生し、音色データを表示します。カーソルバーがディレクトリを指していれば、そのディレクトリに移動します。

- `SPACE`

カーソルバーの位置のMDXを再生し、音色データを表示すると共にカーソルバーを一つ下に移動します。ディレクトリ移動は行いません。

- `BS`

カーソルバーの位置のMDXを再生し、音色データを表示すると共にカーソルバーを一つ上に移動します。ディレクトリ移動は行いません。

- `TAB`

一つ後の音色データを表示します。

- `SHIFT+TAB`

一つ前の音色データを表示します。

- `RIGHT`

一つ後のドライブを選択します。

- `LEFT`

一つ前のドライブを選択します。

- `UP`
- `k`
- `CTRL+p`

カーソルバーを上に1つ移動します。

- `DOWN`
- `j`
- `CTRL+n`

カーソルバーを下に1つ移動します。

- `ROLLUP`
- `CTRL+f`

ページ送りを行います。

- `ROLLDOWN`
- `CTRL+b`
- `b`

ページ戻しを行います。

- `HOME`
- `<`

リストの先頭にジャンプします。

- `>`

リストの最後にジャンプします。

- `r`

リストのソート順を反転させます。

- `.`

一つ上のディレクトリに移動します。

- `\`

ルートディレクトリに移動します。

- `s`

曲を中断します。

- `c`

曲を再開します。

- `f`

曲をフェードアウトします。

- `p`

曲を再生します。

- `e`

曲を終了します。

- `a`

自動演奏をON/OFFします。

- `S`

シャッフルモードをON/OFFします。大文字 `S` (`SHIFT+s`) です。

- `M`

大文字 `M` (`SHIFT+m`)です。MMDSP を使って現在のカーソル位置の曲を演奏します。MMDSP を終了させると MDXVV に戻ります。

- `x`

音色データのエクスポートを行います。

---

## 音色データのエクスポート / Voice Data Export

to be documented

---

## Special Thanks

開発にあたり以下を利用させて頂きました。ありがとうございました。

* クロスコンパイル
  * [xdev68k](https://github.com/yosshin4004/xdev68k/) ([gcc 12.2.0に変更](https://github.com/tantanGH/distribution/blob/main/INSTALL_xdev68k_M1Mac.md)) Thanks to ファミべのよっしんさん
  * HAS060.X on run68mac Thanks to yunkさん / M.Kamadaさん / GOROmanさん
  * HLK301.X on run68mac Thanks to SALTさん
  * SHARP XC 2.1 ライブラリ
  * Microsoft Visual Studio Code
  * Apple MacBook Air (2020, Apple Silicon, RAM8GB, SSD512GB)

* エミュレータ
  * [XEiJ](https://stdkmd.net/xeij/) Thanks to M.Kamadaさん

* 実機
  * SHARP X68030 HD (CZ-510C)
  * DELL P2314H モニタ

* 書籍
  * [Inside X68000, 桒野雅彦, ソフトバンク, 1992年](https://amzn.asia/d/6IA3urZ)
  * [サウンドプログラミング入門, 青木直史, 技術評論社, 2013年](https://amzn.asia/d/5Jqxlwu)

* その他
  * [ChatGPT](https://openai.com/blog/chatgpt/)

![](images/mdxvv2.jpeg)

---