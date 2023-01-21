# MDXVV.X (開発中)
MDX Voice Viewer for X680x0/Human68k

MDXセレクタ＆プレーヤー兼音色データビュワー＆抽出アプリケーションです。

注意：現在開発中につきまだ配布パッケージは作っていません。この説明書きだけになります。

![](images/mdxvv1.png)

なお、本アプリケーションでは、音色を表す語として *voice* を使っています。これは SHARP 純正 XC ライブラリに準じています。

---

## インストール / Install

MDXVVxxx.ZIP をダウンロードして展開し、MDXVV.X をパスの通ったディレクトリにコピーします。

また、以下の実行ファイルにパスを通しておく必要があります。

* COMMAND.X
* MXP.X
* MMDSP.r (オプション)

MXDRV の常駐は必須です。

---

## 起動方法 / How To Start

    MDXVV [options] [mdx-directory-name]

MDXファイルが格納されているフォルダを指定して起動します。
指定が無い場合はカレントディレクトリが指定されたものと扱います。

オプションとして以下の指定が可能です。

・`-g`

グラフィック画面の初期化を行いません。壁紙画像を維持したい場合などに。MDXVV自体はテキスト2プレーンのみを使用します。

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

- `M`

MMDSP を使って現在のカーソル位置の曲を演奏します。MMDSP を終了させると MDXVV に戻ります。

- `x`

音色データのエクスポートを行います。

---

## 音色データのエキスポートについて / Voice Data Export


---

## Special Thanks

---