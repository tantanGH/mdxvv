# MDXVV.X
MDX Voice Viewer for X680x0/Human68k

MDXセレクタ＆プレーヤー＆音色データビュワー＆音色データ抽出ソフトです。

<img src='images/mdxvv1.png' width='578'/>

注意：MDXVVで表示される波形は音色パラメータをそれなりに考慮して計算された結果ではありますが、見た目のみで実際には厳密なものではありませんのでご了承下さい。

注意：低クロック機ではもたつきなどが発生します。できるだけXVI(16MHz)、いやRedZone(24MHz)相当以上でお使いください...。メモリも最低4MBは必要です。エミュレータ上ではMC68000 24MHz/4MBあればほぼこちらの期待する最低限の動きになることを確認しています。推奨は060turboまたはPhantomXです(ぉ

---

## Install / インストール

[MDXVV080.ZIP](https://github.com/tantanGH/mdxvv/raw/main/MDXVV080.ZIP) をダウンロードして展開し、MDXVV.X をパスの通ったディレクトリにコピーします。

MXDRVの常駐が必要です。MXDRVバージョン2.06+16相当が常駐していない場合はエラーメッセージが出てMDXVVは起動しません。

* [MXDRV.X](http://retropc.net/x68000/software/sound/mxdrv/)

画面の描画にIOCSコールを多用しているので、HIOCS.XなどIOCSを高速化するソフトウェアの導入を強く推奨します。

* [HIOCS.X](http://retropc.net/x68000/software/system/iocs/hiocs/)

---

## How To Start / 起動方法

    mdxvv [mdx-directory-name]

MDXファイルが格納されているフォルダを指定して起動します。
指定が無い場合はカレントディレクトリが指定されたものとして扱います。
MDXファイルそのものを指定しても構いません。その場合はそのMDXファイルがあるフォルダからの起動になります。

<img src='images/mdxvv5a.gif'/>

---

## Key Bindings / 操作方法

操作はすべてキーボードで行います。

<br/>

- `h`

主要な操作に関するヘルプメッセージを表示します。

<br/>

- `ESC`
- `q`

MDXVVを終了します。

<br/>

- `CR`
- `ENTER`
- `CTRL+m`

カーソルバーの位置のMDXを再生し、音色データを表示します。カーソルバーがディレクトリを指していれば、そのディレクトリに移動します。

<br/>

- `SPACE`

カーソルバーの位置のMDXを再生し、音色データを表示すると共にカーソルバーを一つ下に移動します。ディレクトリ移動は行いません。

<br/>

- `BS`

カーソルバーの位置のMDXを再生し、音色データを表示すると共にカーソルバーを一つ上に移動します。ディレクトリ移動は行いません。

<br/>

- `TAB`

一つ後の音色データを表示します。

<br/>

- `SHIFT+TAB`

一つ前の音色データを表示します。

<br/>

- `RIGHT`

一つ後のドライブを選択します。

<br/>

- `LEFT`

一つ前のドライブを選択します。

<br/>

- `UP`
- `k`
- `CTRL+p`

カーソルバーを上に1つ移動します。

<br/>

- `DOWN`
- `j`
- `CTRL+n`

カーソルバーを下に1つ移動します。

<br/>

- `ROLLUP`
- `CTRL+f`
- `CTRL+v`

ページ送りを行います。

<br/>

- `ROLLDOWN`
- `CTRL+b`
- `b`

ページ戻しを行います。

<br/>

- `HOME`
- `<`

リストの先頭にジャンプします。

<br/>

- `UNDO`
- `>`

リストの最後にジャンプします。

<br/>

- `r`
- `R`

リストのソート順を反転させます。

<br/>

- `.`

一つ上のディレクトリに移動します。

<br/>

- `\`

ルートディレクトリに移動します。

<br/>

- `s`

曲を中断します。

<br/>

- `c`

曲を再開します。

<br/>

- `f`

曲をフェードアウトします。自動演奏モードの場合はフェードアウト完了後に次の曲に進みます。

<br/>

- `p`

曲を再生します。

<br/>

- `e`

曲を終了します。自動演奏モードの場合は次の曲に進みます。

<br/>

- `a`

自動演奏をON/OFFします。

<br/>

- `A`

大文字 `A` (`SHIFT+a`) です。波形アニメーション表示をON/OFFします。ただしすべての波形計算をまとめて行うので低クロック機では辛くなります。

<br/>

- `S`

大文字 `S` (`SHIFT+s`) です。シャッフルモードをON/OFFします。

<br/>

- `M`

大文字 `M` (`SHIFT+m`)です。MMDSP を起動します。MMDSP を終了させると MDXVV に戻ります。MMDSP.r および COMMAND.X にパスを通しておく必要があります。

<br/>

- `x`
- `X`

音色データのエクスポートを行います。

---

## Voice Data Export / 音色データのエクスポート

音色データが表示されている状態で `x` キーを押すことで、音色データをファイルに書き出すことができます。

<img src='images/mdxvv3.png' width='576'/>

フォーマットは4通り選択できます。

1. MDX
2. ZMS (Z-MUSIC)
3. XC
4. BAS (X-BASIC)

`1`から`4`のいずれかのキーで選択してください。なお、ハードウェアLFO関連パラメータは一切無視されます。


<img src='images/mdxvv4.png' width='576'/>

続いて書き出し先のファイル名を直接入力してください。日本語フォルダ・日本語ファイル名は使わないでください。既に既存のファイルがある場合は確認を求められます。

出力される音色データは曲データに含まれるすべての音色になります。

---

## Limitations / 制約事項

日本語の含まれるファイル名は再生・音色表示は可能ですが、画面上正しく表示できません。曲タイトルについては表示可能です。

---

## 変更履歴

- version 0.8.0 ... 低クロック機向け改善。ただしX68000Zではかなり厳しい
- version 0.7.0 ... ハイメモリドライバの有無チェック ハイメモリの使い方見直し MXDRVの常駐チェック方式変更
- version 0.6.1 ... 公開

---

## Special Thanks

開発にあたり以下のソフトウェア・ハードウェア・リソースを利用および参考にさせて頂きました。この場を借りてお礼申し上げます。

* クロスコンパイル
  * [xdev68k](https://github.com/yosshin4004/xdev68k/) ([gcc 12.2.0に変更](https://github.com/tantanGH/distribution/blob/main/INSTALL_xdev68k_M1Mac.md)) Thanks to ファミべのよっしんさん
  * HAS060.X on run68mac Thanks to YuNKさん / M.Kamadaさん / GOROmanさん
  * HLK301.X on run68mac Thanks to SALTさん
  * SHARP XC 2.1 ライブラリ

* エミュレータ
  * [XEiJ](https://stdkmd.net/xeij/) Thanks to M.Kamadaさん


<img src='images/mdxvv2.jpeg' width='806'/>


---

## Terms Of Use / 免責

ここで配布されているソフトウェアを使用したことにより何らかの不具合(システムクラッシュその他)が生じても、一切の責任は負えません。自己責任にてご利用ください。
アーカイブの再配布は不具合が見つかった時に収拾がつかなくなるのでなるべくご遠慮ください。ただしこの github のリンクの紹介は自由です。

---

## Contact / 連絡先

tantan
 - GitHub: https://github.com/tantanGH/
 - Twitter: https://twitter.com/snakGH/

---
