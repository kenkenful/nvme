# NVMe Storport Driver

このリポジトリは Windows 用の NVMe Storport ミニポートドライバーです。現在のプロジェクトは x64 の `Debug` 構成でビルド確認しています。

> 注意: このドライバーは `PCI\CC_010802` の NVMe コントローラーに一致します。インストールするとシステムのストレージデバイスに影響する可能性があります。実機に入れる前に、テスト用 PC または VM、復元手段、管理者権限を用意してください。

## 必要な環境

- Windows
- Visual Studio 2022
- Windows Driver Kit
- 管理者権限の PowerShell または Developer PowerShell

## ビルド方法

Visual Studio の GUI からビルドします。

1. Visual Studio 2022 を起動します。
2. `nvme.sln` を開きます。
3. 画面上部の構成を `Debug`、プラットフォームを `x64` に設定します。
4. ソリューション エクスプローラーで `nvme` プロジェクトを右クリックし、`プロパティ` を開きます。
5. `構成プロパティ` -> `リンカー` -> `入力` を開きます。
6. `追加の依存ファイル` に `storport.lib` が含まれていることを確認します。含まれていない場合は追加します。
7. メニューから `ビルド` -> `ソリューションのビルド` を選択します。

成功すると、主な生成物は次に出力されます。

```text
x64\Debug\nvme.sys
x64\Debug\nvme\nvme.sys
x64\Debug\nvme\nvme.inf
x64\Debug\nvme\nvme64.cat
```

クリーンビルドする場合は、メニューから `ビルド` -> `ソリューションのリビルド` を選択します。

## 署名について

x64 Windows でカーネルドライバーをロードするには、通常は署名が必要です。このプロジェクトはビルド時の署名を無効にしているため、実機ロード用には別途テスト署名または正式署名を行ってください。

テスト署名を使う場合の一般的な流れは次のとおりです。

```powershell
bcdedit /set testsigning on
```

実行後、Windows を再起動します。

テスト証明書を作成して署名する例です。

```powershell
$cert = New-SelfSignedCertificate -Type CodeSigningCert -Subject "CN=NVMe Test Driver" -CertStoreLocation Cert:\LocalMachine\My
Export-Certificate -Cert $cert -FilePath .\NVMeTestDriver.cer
Import-Certificate -FilePath .\NVMeTestDriver.cer -CertStoreLocation Cert:\LocalMachine\TrustedRoot
Import-Certificate -FilePath .\NVMeTestDriver.cer -CertStoreLocation Cert:\LocalMachine\TrustedPublisher
```

`signtool.exe` で catalog に署名します。

```powershell
signtool sign /v /fd SHA256 /s My /n "NVMe Test Driver" /t http://timestamp.digicert.com .\x64\Debug\nvme\nvme64.cat
```

## インストール方法

管理者権限の PowerShell で、リポジトリのルートから実行します。

```powershell
pnputil /add-driver .\x64\Debug\nvme\nvme.inf /install
```

インストール後、必要に応じて再起動してください。

インストールされたドライバーパッケージを確認するには次を実行します。

```powershell
pnputil /enum-drivers
```

`Provider Name` や `Original Name` が `nvme.inf` の項目を探し、`Published Name` の `oemXX.inf` を控えておきます。アンインストール時に使います。

## アンインストール方法

まず、登録済みドライバーパッケージを確認します。

```powershell
pnputil /enum-drivers
```

`Original Name : nvme.inf` の `Published Name` を確認します。たとえば `oem42.inf` だった場合、管理者権限で次を実行します。

```powershell
pnputil /delete-driver oem42.inf /uninstall /force
```

削除後、必要に応じて再起動してください。

テスト署名モードを戻す場合は次を実行して再起動します。

```powershell
bcdedit /set testsigning off
```

## トラブルシュート

ビルドで WDK やドライバー用のツールセットが見つからない場合は、Visual Studio Installer で Windows Driver Kit と C++ によるデスクトップ開発が入っているか確認してください。

`pnputil` で署名エラーになる場合は、テスト署名モードが有効か、`nvme64.cat` が信頼済み証明書で署名されているか確認してください。

インストール後にストレージデバイスが正常に動作しない場合は、回復環境またはデバイスマネージャーから以前のドライバーへ戻してください。
