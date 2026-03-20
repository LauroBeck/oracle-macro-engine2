import pandas as pd

df = pd.read_csv("data/macro_data_v4.csv")

# 🔥 FORCE RENAME (CRITICAL FIX)
df.columns = ["tech","capex","oil","gold","vix","orcl"]

print("Columns fixed:", df.columns)

returns = df.pct_change().dropna()

returns["alpha"] = returns["capex"] - returns["tech"] - returns["oil"]

returns["strategy"] = returns["alpha"].shift(1) * returns["capex"]

equity = (1 + returns["strategy"]).cumprod()

print("\nFinal equity:", equity.iloc[-1])
print("\nLast rows:")
print(returns.tail())
