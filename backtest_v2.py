import pandas as pd

df = pd.read_csv("data/macro_data_v4.csv")
df.columns = ["tech","capex","oil","gold","vix","orcl"]

returns = df.pct_change().dropna()

# alpha
returns["alpha"] = returns["capex"] - returns["tech"] - returns["oil"]

# 🔥 SIGNAL FILTER
returns["signal"] = 0
returns.loc[returns["alpha"] > 0.05, "signal"] = 1
returns.loc[returns["alpha"] < -0.05, "signal"] = -1

# strategy
returns["strategy"] = returns["signal"].shift(1) * returns["capex"]

equity = (1 + returns["strategy"]).cumprod()

print("Final equity:", equity.iloc[-1])
print(returns.tail())
