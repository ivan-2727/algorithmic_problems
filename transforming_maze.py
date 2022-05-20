# My favorite, completely solved: Dijkstra algorithm on a graph which changes with each move
# https://www.codewars.com/kata/5b86a6d7a4dcc13cd900000b

import heapq #priority_queue

oo = 1e10

def rotate(cell, d):
    if d%4 == 0:
        return cell
    newCell = ['' for _ in range(4)]
    if d%4 == 1:
        for i in range(0,3):
            newCell[i] = cell[i+1]
        newCell[3] = cell[0]
    if d%4 == 2:
        for i in range(0,2):
            newCell[i] = cell[i+2]
        newCell[3] = cell[1]
        newCell[2] = cell[0]
    if d%4 == 3:
            newCell[0] = cell[3]
            newCell[1] = cell[0]
            newCell[2] = cell[1]
            newCell[3] = cell[2]
    return newCell

#north, west, south, east

def freeWay(v,u,grid,d):
    cV = rotate(grid[v[0]][v[1]],d)
    cU = rotate(grid[u[0]][u[1]],d)
    if u[0]==v[0]:
        if v[1]>u[1]:
            return cV[1]==0 and cU[3]==0
        else:
            return cU[1]==0 and cV[3]==0
    else:
        if v[0]>u[0]:
            return cU[2]==0 and cV[0]==0
        else:
            return cV[2]==0 and cU[0]==0

def neigh(v, n, m):
    ngh = []
    if v[0]-1>=0:
        ngh.append([v[0]-1,v[1]])
    if v[0]+1<=n-1:
        ngh.append([v[0]+1,v[1]])
    if v[1]-1>=0:
        ngh.append([v[0],v[1]-1])
    if v[1]+1<=m-1:
        ngh.append([v[0],v[1]+1])
    return ngh

def toCell(x):
    cell = [0 for _ in range(4)]
    for i in range(3,-1,-1):
        cell[i] = x%2
        x = x//2
    return cell 

def maze_solver(ar):
    n = len(ar)
    m = len(ar[0])
    grid = [[[] for _ in range(m)] for _ in range(n)]
    s = []
    f = []
    for i in range(n):
        for j in range(m):
            if ar[i][j]=='B':
                s = [i,j]
                grid[i][j] = toCell(0)
            elif ar[i][j]=='X':
                f = [i,j]
                grid[i][j] = toCell(0)
            else:
                grid[i][j] = toCell(ar[i][j])
    
    visit = [[False for _ in range(m)] for _ in range(n)]
    dist = [[oo for _ in range(m)] for _ in range(n)]
    parent = [[[] for _ in range(m)] for _ in range(n)]
    dist[s[0]][s[1]] = 0

    q = []
    for i in range(n):
        for j in range(m):
            q.append((dist[i][j], [i,j]))

    heapq.heapify(q)
    
    while True:
        try:
            v = heapq.heappop(q)[1]
            if not visit[v[0]][v[1]]:
                visit[v[0]][v[1]] = True
                for u in neigh(v, n, m):
                    if not visit[u[0]][u[1]]:
                        upd = dist[v[0]][v[1]]
                        ok = False
                        for k in range(0,4):
                            if freeWay(v,u,grid,dist[v[0]][v[1]]+k):
                                ok = True
                                break
                            upd+=1
                        if upd < dist[u[0]][u[1]] and ok:
                            dist[u[0]][u[1]] = upd
                            parent[u[0]][u[1]] = (v, upd-dist[v[0]][v[1]])
                            heapq.heappush(q, (upd, u))
        except:
            break

    if dist[f[0]][f[1]]==oo:
        print('none')
        return None

    x = f
    path = [(x,0)]
    ps = ""
    while(True):
        if x==s:
            break
        y = parent[x[0]][x[1]]
        path.append(y)
        x = y[0]
    
    path = path[::-1]
    #print(path)

    ps = ""
    for i in range(len(path)-1):
        if path[i][1]>0:
            ps+="1"
            for k in range(path[i][1]-1):
                ps+=" 1"
        if path[i+1][0][0]==path[i][0][0]:
            if path[i+1][0][1]>path[i][0][1]:
                ps += "E"
            else:
                ps += "W"
        else:
            if path[i+1][0][0]>path[i][0][0]:
                ps += "S"
            else:
                ps += "N"

    ans = ps.split('1')
    for i in range(len(ans)):
        if ans[i]==' ':
            ans[i]=''
    
    return ans