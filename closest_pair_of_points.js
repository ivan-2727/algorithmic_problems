// The famous problem "Closest Pair of points on a plane in N log N" 
// https://www.codewars.com/kata/5376b901424ed4f8c20002b7

function dist(a,b) {
    return Math.sqrt(Math.pow(a[0]-b[0],2) + Math.pow(a[1]-b[1],2));
}

function rec(byX, byY) {
    const n = byX.length; 
    if (n==2) {
        return [dist(byX[0],byX[1]),byX];
    }
    if (n==3) {
        let c = []
        c.push([dist(byX[0],byX[1]),[byX[0],byX[1]]]);
        c.push([dist(byX[0],byX[2]),[byX[0],byX[2]]]);
        c.push([dist(byX[1],byX[2]),[byX[1],byX[2]]]);
        c.sort((a,b)=>a[0]-b[0]); 
        return c[0]; 
    }
    const mid = Math.floor(n/2); 
    let byY_left = [];
    let byY_right = [];
    for (let i = 0; i < byY.length; i++) {
        if (byY[i][0] < byX[mid][0]) byY_left.push(byY[i]);
        else byY_right.push(byY[i]); 
    }
    let c = [];
    c.push(rec(byX.slice(0,mid),byY_left));
    c.push(rec(byX.slice(mid,n),byY_right));
    c.sort((a,b)=>a[0]-b[0]);
    const d = c[0][0];
    const left = (byX[mid][0]+byX[mid-1][0])/2-d;
    const right = (byX[mid][0]+byX[mid-1][0])/2+d; 
    let best = d;
    let bestPair = []; 
    for (let i = 0; i < n; i++) {
        if (byY[i][0] > left && byY[i][0] < right) {
            for (let j = i+1; j <= i+6 && j < n; j++) {
                if (byY[j][0] > left && byY[j][0] < right) {
                    let dst = dist(byY[i], byY[j]) 
                    if (dst < best) {
                        best = dst;
                        bestPair = [byY[i], byY[j]];  
                    }
                }
            }
        }
    }
    if (bestPair.length!=0) {
        return[best, bestPair];
    }
    return c[0];
}

function closestPair(pts) {
    const n = pts.length;
    const byX = pts.slice(0,n).sort((a,b)=>{
      if(a[0]!=b[0]) return a[0]-b[0];
      return a[1]-b[1]; 
    });
    for (let i = 1; i < n; i++) {
      if (byX[i][0]==byX[i-1][0] && byX[i][1]==byX[i-1][1]) {
        return [byX[i],byX[i-1]];
      }
    }
    const byY = pts.slice(0,n).sort((a,b)=>a[1]-b[1]); 
    const result = rec(byX, byY);
    return result[1];
}
