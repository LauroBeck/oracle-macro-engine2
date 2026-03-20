import yfinance as yf
import pandas as pd
import os

tickers = {
    "tech": "^IXIC",
    "capex": "^GSPC",
    "oil": "CL=F",
    "gold": "GC=F",
    "vix": "^VIX",
    "orcl": "ORCL"
}

series_list = []

for name, ticker in tickers.items():
    print(f"Fetching {name}...")

    try:
        df = yf.download(ticker, period="1mo", interval="1d", progress=False)

        if df is None or df.empty:
            print(f"⚠️ No data for {name}")
            continue

        s = df["Close"].copy()
        s.name = name

        series_list.append(s)

    except Exception as e:
        print(f"❌ Error {name}: {e}")

if not series_list:
    raise Exception("No data fetched")

df = pd.concat(series_list, axis=1, sort=False)
df = df.ffill().dropna()

os.makedirs("data", exist_ok=True)
df.to_csv("data/macro_data_v4.csv", index=False)

print("\n✅ CLEAN DATA READY")
print(df.tail())
