import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("timings.csv")

fig1, ax1 = plt.subplots(figsize=(7, 5))
for program, group in df.groupby("program"):
    group = group.sort_values("processes")
    ax1.plot(group["processes"], group["time_seconds"], marker="o", label=program)
ax1.set_xlabel("Number of Processors")
ax1.set_ylabel("Time (seconds)")
ax1.set_title("Time vs Number of Processors")
ax1.legend()
ax1.grid(True)
fig1.savefig("time_vs_processors.png", dpi=150, bbox_inches="tight")

fig2, ax2 = plt.subplots(figsize=(7, 5))
for program, group in df.groupby("program"):
    group = group.sort_values("processes")
    t1 = group.loc[group["processes"] == 1, "time_seconds"].values[0]
    speedup = t1 / group["time_seconds"]
    ax2.plot(group["processes"], speedup, marker="o", label=program)

# Ideal (linear) speedup line for reference
max_p = df["processes"].max()
ax2.plot([1, max_p], [1, max_p], linestyle="--", color="gray", label="Ideal speedup")

ax2.set_xlabel("Number of Processors")
ax2.set_ylabel("Speedup (T1 / Tp)")
ax2.set_title("Speedup vs Number of Processors")
ax2.legend()
ax2.grid(True)
fig2.savefig("speedup.png", dpi=150, bbox_inches="tight")

print("Saved time_vs_processors.png and speedup.png")
