//The task was to create a JS function which would mimic chainable SQL queries 
//Top level problem on Codewars https://www.codewars.com/kata/545434090294935e7d0010ab

var query = function() {
    
    let q = {};

    q.data = []; 

    q.totalSelects = 0;
    q.totalFroms = 0; 
    q.totalOrderBys = 0; 
    q.totalGroupBys = 0;

    q.groupParameter = []; 
    q.whereParameter = []; 
    q.depth = 0;

    q.handleGroup = function(dat, groupby) {
        let all = [];
        dat.forEach(item => {
            let term = groupby(item);
            if (!all.includes(term)) all.push(term); 
        })
        let res = [];
        all.forEach(term => {
            let tmp = [term, []];
            dat.forEach(item => {
                if (groupby(item) == term) {
                    tmp[1].push(item); 
                }
            });
            res.push(tmp);
        });
        
        return res;
    }

    q.recGroup = (dat, k) => {
        if (k == q.groupParameter.length) {
            return dat;
        }
        dat = q.handleGroup(dat, q.groupParameter[k]); 
        for (let i = 0; i < dat.length; i++) {
            dat[i] = [dat[i][0], q.recGroup(dat[i][1], k+1)]; 
        }
        return dat; 
    }
 
    q.select = function() {
        q.totalSelects++;
        if (q.totalSelects > 1) throw Error('Duplicate SELECT')
        if (arguments[0]) q.selectParameter = arguments[0]; 
        return q; 
    }
    
    q.from = function() {
        q.totalFroms++;
        if (q.totalFroms > 1) throw Error ('Duplicate FROM'); 
        if (arguments.length==1) {
            q.data = q.data.concat(arguments[0]);
        }
        if (arguments.length==2) {
            for (let i = 0; i < arguments[0].length; i++) {
                for (let j = 0; j < arguments[1].length; j++) {
                    q.data.push([arguments[0][i], arguments[1][j]]); 
                }
            }
        }
        return q;
    }

    q.where = function() {
        if (arguments[0]) {
            let args = Object.keys(arguments);
            let tmp = []; 
            for (let i = 0; i < args.length; i++) tmp.push(arguments[args[i]]); 
            q.whereParameter.push(tmp); 
        }
        return q;
    }

    q.orderBy = function() {
        q.totalOrderBys++;
        if (q.totalOrderBys > 1) throw Error ('Duplicate ORDERBY');
        if (arguments[0]) q.orderParameter = arguments[0];
        return q;
    }

    q.groupBy = function() {
        q.totalGroupBys++;
        if (q.totalGroupBys > 1) throw Error ('Duplicate GROUPBY');
        let args = Object.keys(arguments);
        for (let i = 0; i < args.length; i++) q.groupParameter.push(arguments[args[i]]);
        return q;
    }

    q.having = function() {
        if (arguments[0]) q.havingParameter = arguments[0]; 
        return q;
    }

    q.execute = function() {

        if (q.whereParameter.length > 0) {
            q.data = q.data.filter(item => {
                let gen_ok = true;
                for (let i = 0; i < q.whereParameter.length; i++) {
                    let part_ok = false;
                    for (let j = 0; j < q.whereParameter[i].length; j++) {
                        part_ok = part_ok || q.whereParameter[i][j](item); 
                    }
                    gen_ok = gen_ok && part_ok; 
                }
                return gen_ok;
            })

        }


        if (q.groupParameter.length > 0) {
            q.data = q.recGroup(q.data, 0); 
        }

        if (q.havingParameter) {
            q.data = q.data.filter(group => q.havingParameter(group))
        }

        if (q.selectParameter) q.data = q.data.map(item => q.selectParameter(item));

        if (q.orderParameter) q.data.sort(q.orderParameter);

        return q.data; 
    }

    return q;
}