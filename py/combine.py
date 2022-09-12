import networkx as nx
import sys
import subprocess

def alpha(G):
    return nx.graph_clique_number(nx.complement(G))

def tau(G):
    return G.order() - alpha(G)

def short(L):
    bts = b""
    for G in L:
        bts += nx.to_graph6_bytes(G)[10:]
    p = subprocess.run(["shortg"], input=bts, capture_output=True)
    gs = p.stdout.split(b"\n")
    return [nx.from_graph6_bytes(gb) for gb in gs if gb != b""]

connected = dict()
for line in sys.stdin:
    if line[-1] == "\n":
        line = line[:-1]
    G = nx.from_graph6_bytes(line.encode())
    t = tau(G)
    if t in connected:
        connected[t] += [G]
    else:
        connected[t] = [G]

for k in connected:
    print("{}: {}".format(k, len(connected[k])))

print("---------")

uc = dict()
ks = sorted(list(connected.keys()))
uc[ks[0]] = connected[ks[0]]
for k in range(ks[0], ks[-1]+1):
    if k in connected:
        uc[k] = connected[k]
    else:
        uc[k] = []
    i = ks[0]
    j = k-ks[0]
    while i <= j:
        if (i in uc.keys()) and (j in uc.keys()):
            if i != j:
                for ig in uc[i]:
                    for jg in uc[j]:
                        uc[k] += [nx.disjoint_union(ig, jg)]
            else:
                for igi in range(len(uc[i])):
                    for jgi in range(igi,len(uc[j])):
                        uc[k] += [nx.disjoint_union(uc[i][igi], uc[j][jgi])]
        i += 1
        j -= 1

for k in uc:
    sl = short(uc[k])
    gss = [nx.to_graph6_bytes(g)[10:-1].decode() for g in sl]
    f = open("taucrit_{}.g6.txt".format(k), "w")
    for gs in gss:
        print(gs, file=f)
    f.close()
