/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @constructor
 * @extends {WebInspector.DataGridNode}
 * @param {WebInspector.HeapSnapshotSortableDataGrid} tree
 * @param {boolean} hasChildren
 * @param {WebInspector.HeapSnapshotProviderProxy|WebInspector.HeapSnapshotDiffNodesProvider} provider
 */
WebInspector.HeapSnapshotGridNode = function(tree, hasChildren, provider)
{
    WebInspector.DataGridNode.call(this, null, hasChildren);
    /**
     * @type {WebInspector.HeapSnapshotProviderProxy|WebInspector.HeapSnapshotDiffNodesProvider}
     */
    this._provider = provider;
    this._dataGrid = tree;
    this._instanceCount = 0;
    this.addEventListener("populate", this._populate, this);
}

WebInspector.HeapSnapshotGridNode.prototype = {
    createCell: function(columnIdentifier)
    {
        var cell = WebInspector.DataGridNode.prototype.createCell.call(this, columnIdentifier);
        if (this._searchMatched)
            cell.addStyleClass("highlight");
        return cell;
    },

    collapse: function()
    {
        WebInspector.DataGridNode.prototype.collapse.call(this);
        this._dataGrid.updateVisibleNodes();
    },

    dispose: function()
    {
        if (this._provider)
            this._provider.dispose();
        for (var node = this.children[0]; node; node = node.traverseNextNode(true, this, true))
            if (node.dispose)
                node.dispose();
    },

    hasHoverMessage: false,

    queryObjectContent: function(callback)
    {
    },

    _toPercentString: function(num)
    {
        return num.toFixed(0) + "\u2009%"; // \u2009 is a thin space.
    },

    _createValueCell: function(columnIdentifier)
    {
        var cell = document.createElement("td");
        cell.className = columnIdentifier + "-column";
        if (this.dataGrid.snapshot.totalSize !== 0) {
            var div = document.createElement("div");
            var valueSpan = document.createElement("span");
            valueSpan.textContent = this.data[columnIdentifier];
            div.appendChild(valueSpan);
            var percentColumn = columnIdentifier + "-percent";
            if (percentColumn in this.data) {
                var percentSpan = document.createElement("span");
                percentSpan.className = "percent-column";
                percentSpan.textContent = this.data[percentColumn];
                div.appendChild(percentSpan);
            }
            cell.appendChild(div);
        }
        return cell;
    },

    _populate: function(event)
    {
        this.removeEventListener("populate", this._populate, this);
        function sorted(ignored)
        {
            this._populateChildren();
        }
        this._provider.sortAndRewind(this.comparator(), sorted.bind(this));
    },

    /**
     * @param {?number} howMany
     * @param {?number} atIndex
     */
    _populateChildren: function(howMany, atIndex, afterPopulate)
    {
        howMany = howMany || this._dataGrid.defaultPopulateCount();
        atIndex = atIndex || this.children.length;
        var haveSavedChildren = !!this._savedChildren;
        if (haveSavedChildren) {
            haveSavedChildren = false;
            for (var c in this._savedChildren) {
                haveSavedChildren = true;
                break;
            }
        }

        var part = 0;
        function callSerialize()
        {
            if (part >= howMany)
                return;
            part += this._dataGrid.defaultPopulateCount();
            this._provider.serializeSubsequentItems(this._dataGrid.defaultPopulateCount(), childrenRetrieved.bind(this));
        }
        function childrenRetrieved(items)
        {
            var length = items.totalLength;
            for (var i = 0, l = items.length; i < l; ++i) {
                var item = items[i];
                if (haveSavedChildren) {
                    var hash = this._childHashForEntity(item);
                    if (hash in this._savedChildren) {
                        this.insertChild(this._savedChildren[hash], atIndex++);
                        continue;
                    }
                }
                this.insertChild(this._createChildNode(item), atIndex++);
            }
            this._instanceCount += items.length;
            if (part < howMany) {
                setTimeout(callSerialize.bind(this), 0);
                return;
            }

            if (items.hasNext)
                this.insertChild(new WebInspector.ShowMoreDataGridNode(this._populateChildren.bind(this), this._dataGrid.defaultPopulateCount(), length), atIndex++);
            if (afterPopulate)
                afterPopulate();
            function notify()
            {
                this.dispatchEventToListeners("populate complete");
            }
            setTimeout(notify.bind(this), 0);
        }
        setTimeout(callSerialize.bind(this), 0);
    },

    _saveChildren: function()
    {
        this._savedChildren = {};
        for (var i = 0, childrenCount = this.children.length; i < childrenCount; ++i) {
            var child = this.children[i];
            if (child.expanded)
                this._savedChildren[this._childHashForNode(child)] = child;
        }
    },

    sort: function()
    {
        this._dataGrid.recursiveSortingEnter();
        function afterSort(sorted)
        {
            if (!sorted) {
                this._dataGrid.recursiveSortingLeave();
                return;
            }
            this._saveChildren();
            this.removeChildren();

            function afterPopulate()
            {
                for (var i = 0, l = this.children.length; i < l; ++i) {
                    var child = this.children[i];
                    if (child.expanded)
                        child.sort();
                }
                this._dataGrid.recursiveSortingLeave();
            }
            var instanceCount = this._instanceCount;
            this._instanceCount = 0;
            this._populateChildren(instanceCount, null, afterPopulate.bind(this));
        }
        this._provider.sortAndRewind(this.comparator(), afterSort.bind(this));
    }
};

WebInspector.HeapSnapshotGridNode.prototype.__proto__ = WebInspector.DataGridNode.prototype;

/**
 * @constructor
 * @extends {WebInspector.HeapSnapshotGridNode}
 * @param {WebInspector.HeapSnapshotProviderProxy} provider
 */
WebInspector.HeapSnapshotGenericObjectNode = function(tree, node, provider)
{
    WebInspector.HeapSnapshotGridNode.call(this, tree, false, provider);
    // node is null for DataGrid root nodes.
    if (!node)
        return;
    this._name = node.name;
    this._type = node.type;
    this._distanceToWindow = node.distanceToWindow;
    this._shallowSize = node.selfSize;
    this._retainedSize = node.retainedSize;
    this.snapshotNodeId = node.id;
    this.snapshotNodeIndex = node.nodeIndex;
    if (this._type === "string")
        this.hasHoverMessage = true;
    else if (this._type === "object" && this.isWindow(this._name)) {
        this._name = this.shortenWindowURL(this._name, false);
        this.hasHoverMessage = true;
    } else if (node.flags & tree.snapshot.nodeFlags.canBeQueried)
        this.hasHoverMessage = true;
    if (node.flags & tree.snapshot.nodeFlags.detachedDOMTreeNode)
        this.detachedDOMTreeNode = true;
};

WebInspector.HeapSnapshotGenericObjectNode.prototype = {
    createCell: function(columnIdentifier)
    {
        var cell = columnIdentifier !== "object" ? this._createValueCell(columnIdentifier) : this._createObjectCell();
        if (this._searchMatched)
            cell.addStyleClass("highlight");
        return cell;
    },

    _createObjectCell: function()
    {
        var cell = document.createElement("td");
        cell.className = "object-column";
        var div = document.createElement("div");
        div.className = "source-code event-properties";
        div.style.overflow = "visible";
        var data = this.data["object"];
        if (this._prefixObjectCell)
            this._prefixObjectCell(div, data);
        var valueSpan = document.createElement("span");
        valueSpan.className = "value console-formatted-" + data.valueStyle;
        valueSpan.textContent = data.value;
        div.appendChild(valueSpan);
        var idSpan = document.createElement("span");
        idSpan.className = "console-formatted-id";
        idSpan.textContent = " @" + data["nodeId"];
        div.appendChild(idSpan);
        if (this._postfixObjectCell)
            this._postfixObjectCell(div, data);
        cell.appendChild(div);
        cell.addStyleClass("disclosure");
        if (this.depth)
            cell.style.setProperty("padding-left", (this.depth * this.dataGrid.indentWidth) + "px");
        return cell;
    },

    get data()
    {
        var data = this._emptyData();

        var value = this._name;
        var valueStyle = "object";
        switch (this._type) {
        case "string":
            value = "\"" + value + "\"";
            valueStyle = "string";
            break;
        case "regexp":
            value = "/" + value + "/";
            valueStyle = "string";
            break;
        case "closure":
            value = "function" + (value ? " " : "") + value + "()";
            valueStyle = "function";
            break;
        case "number":
            valueStyle = "number";
            break;
        case "hidden":
            valueStyle = "null";
            break;
        case "array":
            if (!value)
                value = "[]";
            else
                value += "[]";
            break;
        };
        if (this.hasHoverMessage)
            valueStyle += " highlight";
        if (value === "Object")
            value = "";
        if (this.detachedDOMTreeNode)
            valueStyle += " detached-dom-tree-node";
        data["object"] = { valueStyle: valueStyle, value: value, nodeId: this.snapshotNodeId };

        var view = this.dataGrid.snapshotView;
        data["distanceToWindow"] =  this._distanceToWindow;
        data["shallowSize"] = Number.withThousandsSeparator(this._shallowSize);
        data["retainedSize"] = Number.withThousandsSeparator(this._retainedSize);
        data["shallowSize-percent"] = this._toPercentString(this._shallowSizePercent);
        data["retainedSize-percent"] = this._toPercentString(this._retainedSizePercent);

        return this._enhanceData ? this._enhanceData(data) : data;
    },

    queryObjectContent: function(callback, objectGroupName)
    {
        if (this._type === "string")
            callback(WebInspector.RemoteObject.fromPrimitiveValue(this._name));
        else {
            function formatResult(error, object)
            {
                if (!error && object.type)
                    callback(WebInspector.RemoteObject.fromPayload(object), !!error);
                else
                    callback(WebInspector.RemoteObject.fromPrimitiveValue(WebInspector.UIString("Not available")));
            }
            ProfilerAgent.getObjectByHeapObjectId(this.snapshotNodeId, objectGroupName, formatResult);
        }
    },

    get _retainedSizePercent()
    {
        return this._retainedSize / this.dataGrid.snapshot.totalSize * 100.0;
    },

    get _shallowSizePercent()
    {
        return this._shallowSize / this.dataGrid.snapshot.totalSize * 100.0;
    },

    updateHasChildren: function()
    {
        function isEmptyCallback(isEmpty)
        {
            this.hasChildren = !isEmpty;
        }
        this._provider.isEmpty(isEmptyCallback.bind(this));
    },

    isWindow: function(fullName)
    {
        return fullName.substr(0, 9) === "Window";
    },

    shortenWindowURL: function(fullName, hasObjectId)
    {
        var startPos = fullName.indexOf("/");
        var endPos = hasObjectId ? fullName.indexOf("@") : fullName.length;
        if (startPos !== -1 && endPos !== -1) {
            var fullURL = fullName.substring(startPos + 1, endPos).trimLeft();
            var url = fullURL.trimURL();
            if (url.length > 40)
                url = url.trimMiddle(40);
            return fullName.substr(0, startPos + 2) + url + fullName.substr(endPos);
        } else
            return fullName;
    }
}

WebInspector.HeapSnapshotGenericObjectNode.prototype.__proto__ = WebInspector.HeapSnapshotGridNode.prototype;

/**
 * @constructor
 * @extends {WebInspector.HeapSnapshotGenericObjectNode}
 * @param {WebInspector.HeapSnapshotSortableDataGrid} tree
 * @param {boolean} isFromBaseSnapshot
 */
WebInspector.HeapSnapshotObjectNode = function(tree, isFromBaseSnapshot, edge, parentGridNode)
{
    WebInspector.HeapSnapshotGenericObjectNode.call(this, tree, edge.node,
        WebInspector.HeapSnapshotObjectNode._createProvider(isFromBaseSnapshot, edge.nodeIndex, tree));
    this._referenceName = edge.name;
    this._referenceType = edge.type;
    this._propertyAccessor = edge.propertyAccessor;
    this._distanceToWindow = edge.distanceToWindow;
    this.showRetainingEdges = tree.showRetainingEdges;
    this._isFromBaseSnapshot = isFromBaseSnapshot;

    this._parentGridNode = parentGridNode;
    this._cycledWithAncestorGridNode = this._findAncestorWithSameSnapshotNodeId();
    if (!this._cycledWithAncestorGridNode)
        this.updateHasChildren();
}

WebInspector.HeapSnapshotObjectNode._createProvider = function(isFromBaseSnapshot, nodeIndex, tree)
{
    var showHiddenData = WebInspector.settings.showHeapSnapshotObjectsHiddenProperties.get();
    var filter = "function(edge) {\n" +
        "    return !edge.isInvisible\n" +
        "        && (" + !tree.showRetainingEdges + " || (edge.node.id !== 1 && !edge.node.isSynthetic))\n" +
        "        && (" + showHiddenData + " || (!edge.isHidden && !edge.node.isHidden));\n" +
        "}\n";
    var snapshot = isFromBaseSnapshot ? tree.baseSnapshot : tree.snapshot;
    if (tree.showRetainingEdges)
        return snapshot.createRetainingEdgesProvider(nodeIndex, filter);
    else
        return snapshot.createEdgesProvider(nodeIndex, filter);
}

WebInspector.HeapSnapshotObjectNode.prototype = {
    _findAncestorWithSameSnapshotNodeId: function()
    {
        var ancestor = this._parentGridNode;
        while (ancestor) {
            if (ancestor.snapshotNodeId === this.snapshotNodeId)
                return ancestor;
            ancestor = ancestor._parentGridNode;
        }
        return null;
    },

    _createChildNode: function(item)
    {
        return new WebInspector.HeapSnapshotObjectNode(this._dataGrid, this._isFromBaseSnapshot, item, this);
    },

    _childHashForEntity: function(edge)
    {
        var prefix = this.showRetainingEdges ? edge.node.id + "#" : "";
        return prefix + edge.type + "#" + edge.name;
    },

    _childHashForNode: function(childNode)
    {
        var prefix = this.showRetainingEdges ? childNode.snapshotNodeId + "#" : "";
        return prefix + childNode._referenceType + "#" + childNode._referenceName;
    },

    comparator: function()
    {
        var sortAscending = this._dataGrid.sortOrder === "ascending";
        var sortColumnIdentifier = this._dataGrid.sortColumnIdentifier;
        var sortFields = {
            object: ["!edgeName", sortAscending, "retainedSize", false],
            count: ["!edgeName", true, "retainedSize", false],
            shallowSize: ["selfSize", sortAscending, "!edgeName", true],
            retainedSize: ["retainedSize", sortAscending, "!edgeName", true],
            distanceToWindow: ["distanceToWindow", sortAscending, "_name", true]
        }[sortColumnIdentifier] || ["!edgeName", true, "retainedSize", false];
        return WebInspector.HeapSnapshotFilteredOrderedIterator.prototype.createComparator(sortFields);
    },

    _emptyData: function()
    {
        return { count: "", addedCount: "", removedCount: "", countDelta: "", addedSize: "", removedSize: "", sizeDelta: "" };
    },

    _enhanceData: function(data)
    {
        var name = this._referenceName;
        if (name === "") name = "(empty)";
        var nameClass = "name";
        switch (this._referenceType) {
        case "context":
            nameClass = "console-formatted-number";
            break;
        case "internal":
        case "hidden":
            nameClass = "console-formatted-null";
            break;
        case "element":
            name = "[" + name + "]";
            break;
        }
        data["object"].nameClass = nameClass;
        data["object"].name = name;
        data["distanceToWindow"] = this._distanceToWindow;
        return data;
    },

    _prefixObjectCell: function(div, data)
    {
        if (this._cycledWithAncestorGridNode)
            div.className += " cycled-ancessor-node";

        var nameSpan = document.createElement("span");
        nameSpan.className = data.nameClass;
        nameSpan.textContent = data.name;
        div.appendChild(nameSpan);

        var separatorSpan = document.createElement("span");
        separatorSpan.className = "grayed";
        separatorSpan.textContent = this.showRetainingEdges ? " in " : " :: ";
        div.appendChild(separatorSpan);
    }
}

WebInspector.HeapSnapshotObjectNode.prototype.__proto__ = WebInspector.HeapSnapshotGenericObjectNode.prototype;

/**
 * @constructor
 * @extends {WebInspector.HeapSnapshotGenericObjectNode}
 */
WebInspector.HeapSnapshotInstanceNode = function(tree, baseSnapshot, snapshot, node)
{
    WebInspector.HeapSnapshotGenericObjectNode.call(this, tree, node,
        WebInspector.HeapSnapshotInstanceNode._createProvider(baseSnapshot || snapshot, node.nodeIndex));
    this._isDeletedNode = !!baseSnapshot;
    this.updateHasChildren();
};

WebInspector.HeapSnapshotInstanceNode._createProvider = function(snapshot, nodeIndex)
{
    var showHiddenData = WebInspector.settings.showHeapSnapshotObjectsHiddenProperties.get();
    return snapshot.createEdgesProvider(
        nodeIndex,
        "function(edge) {" +
        "    return !edge.isInvisible" +
        "        && (" + showHiddenData + " || (!edge.isHidden && !edge.node.isHidden));" +
        "}");
}

WebInspector.HeapSnapshotInstanceNode.prototype = {
    _createChildNode: function(item)
    {
        return new WebInspector.HeapSnapshotObjectNode(this._dataGrid, this._isDeletedNode, item, null);
    },

    _childHashForEntity: function(edge)
    {
        return edge.type + "#" + edge.name;
    },

    _childHashForNode: function(childNode)
    {
        return childNode._referenceType + "#" + childNode._referenceName;
    },

    comparator: function()
    {
        var sortAscending = this._dataGrid.sortOrder === "ascending";
        var sortColumnIdentifier = this._dataGrid.sortColumnIdentifier;
        var sortFields = {
            object: ["!edgeName", sortAscending, "retainedSize", false],
            distanceToWindow: ["distanceToWindow", sortAscending, "retainedSize", false],
            count: ["!edgeName", true, "retainedSize", false],
            addedSize: ["selfSize", sortAscending, "!edgeName", true],
            removedSize: ["selfSize", sortAscending, "!edgeName", true],
            shallowSize: ["selfSize", sortAscending, "!edgeName", true],
            retainedSize: ["retainedSize", sortAscending, "!edgeName", true]
        }[sortColumnIdentifier] || ["!edgeName", true, "retainedSize", false];
        return WebInspector.HeapSnapshotFilteredOrderedIterator.prototype.createComparator(sortFields);
    },

    _emptyData: function()
    {
        return {count:"", countDelta:"", sizeDelta: ""};
    },

    _enhanceData: function(data)
    {
        if (this._isDeletedNode) {
            data["addedCount"] = "";
            data["addedSize"] = "";
            data["removedCount"] = "\u2022";
            data["removedSize"] = Number.withThousandsSeparator(this._shallowSize);
        } else {
            data["addedCount"] = "\u2022";
            data["addedSize"] = Number.withThousandsSeparator(this._shallowSize);
            data["removedCount"] = "";
            data["removedSize"] = "";
        }
        return data;
    },

    get isDeletedNode()
    {
        return this._isDeletedNode;
    }
}

WebInspector.HeapSnapshotInstanceNode.prototype.__proto__ = WebInspector.HeapSnapshotGenericObjectNode.prototype;

/**
 * @constructor
 * @extends {WebInspector.HeapSnapshotGridNode}
 */
WebInspector.HeapSnapshotConstructorNode = function(tree, className, aggregate, aggregatesKey)
{
    WebInspector.HeapSnapshotGridNode.call(this, tree, aggregate.count > 0,
        tree.snapshot.createNodesProviderForClass(className, aggregatesKey));
    this._name = className;
    this._distanceToWindow = aggregate.distanceToWindow;
    this._count = aggregate.count;
    this._shallowSize = aggregate.self;
    this._retainedSize = aggregate.maxRet;
}

WebInspector.HeapSnapshotConstructorNode.prototype = {
    createCell: function(columnIdentifier)
    {
        var cell = columnIdentifier !== "object" ? this._createValueCell(columnIdentifier) : WebInspector.HeapSnapshotGridNode.prototype.createCell.call(this, columnIdentifier);
        if (this._searchMatched)
            cell.addStyleClass("highlight");
        return cell;
    },

    _createChildNode: function(item)
    {
        return new WebInspector.HeapSnapshotInstanceNode(this._dataGrid, null, this._dataGrid.snapshot, item);
    },

    comparator: function()
    {
        var sortAscending = this._dataGrid.sortOrder === "ascending";
        var sortColumnIdentifier = this._dataGrid.sortColumnIdentifier;
        var sortFields = {
            object: ["id", sortAscending, "retainedSize", false],
            distanceToWindow: ["distanceToWindow", true, "retainedSize", false],
            count: ["id", true, "retainedSize", false],
            shallowSize: ["selfSize", sortAscending, "id", true],
            retainedSize: ["retainedSize", sortAscending, "id", true]
        }[sortColumnIdentifier];
        return WebInspector.HeapSnapshotFilteredOrderedIterator.prototype.createComparator(sortFields);
    },

    _childHashForEntity: function(node)
    {
        return node.id;
    },

    _childHashForNode: function(childNode)
    {
        return childNode.snapshotNodeId;
    },

    get data()
    {
        var data = { object: this._name };
        var view = this.dataGrid.snapshotView;
        data["count"] =  Number.withThousandsSeparator(this._count);
        data["distanceToWindow"] =  this._distanceToWindow;
        data["shallowSize"] = Number.withThousandsSeparator(this._shallowSize);
        data["retainedSize"] = Number.withThousandsSeparator(this._retainedSize);
        data["count-percent"] =  this._toPercentString(this._countPercent);
        data["shallowSize-percent"] = this._toPercentString(this._shallowSizePercent);
        data["retainedSize-percent"] = this._toPercentString(this._retainedSizePercent);
        return data;
    },

    get _countPercent()
    {
        return this._count / this.dataGrid.snapshot.nodeCount * 100.0;
    },

    get _retainedSizePercent()
    {
        return this._retainedSize / this.dataGrid.snapshot.totalSize * 100.0;
    },

    get _shallowSizePercent()
    {
        return this._shallowSize / this.dataGrid.snapshot.totalSize * 100.0;
    }
};

WebInspector.HeapSnapshotConstructorNode.prototype.__proto__ = WebInspector.HeapSnapshotGridNode.prototype;

/**
 * @constructor
 * @extends {WebInspector.HeapSnapshotProviderProxy}
 * @param {WebInspector.HeapSnapshotProviderProxy} addedNodesProvider
 * @param {WebInspector.HeapSnapshotProviderProxy} deletedNodesProvider
 */
WebInspector.HeapSnapshotDiffNodesProvider = function(addedNodesProvider, deletedNodesProvider, addedCount, removedCount)
{
    this._addedNodesProvider = addedNodesProvider;
    this._deletedNodesProvider = deletedNodesProvider;
    this._addedCount = addedCount;
    this._removedCount = removedCount;
    this._serializedItemsCount = 0;
}

WebInspector.HeapSnapshotDiffNodesProvider.prototype = {
    dispose: function()
    {
        this._addedNodesProvider.dispose();
        this._deletedNodesProvider.dispose();
    },

    isEmpty: function(callback)
    {
        callback(false);
    },

    serializeSubsequentItems: function(count, callback)
    {
        function didReceiveAllItems(items)
        {
            this._serializedItemsCount += items.length;
            items.totalLength = this._addedCount + this._removedCount;
            callback(items);
        }

        function didReceiveDeletedItems(addedItems, items)
        {
            for (var i = 0; i < items.length; i++) {
                items[i].isAddedNotRemoved = false;
                addedItems.push(items[i]);
            }
            addedItems.hasNext = items.hasNext;
            didReceiveAllItems.call(this, addedItems);
        }

        function didReceiveAddedItems(items)
        {
            for (var i = 0; i < items.length; i++)
                items[i].isAddedNotRemoved = true;
            if (items.length < count)
                return this._deletedNodesProvider.serializeSubsequentItems(count - items.length, didReceiveDeletedItems.bind(this, items));

            items.totalLength = this._addedCount + this._removedCount;
            if (!items.hasNext)
                items.hasNext = !!this._removedCount;
            didReceiveAllItems.call(this, items);
        }

        if (this._serializedItemsCount < this._addedCount)
            this._addedNodesProvider.serializeSubsequentItems(count, didReceiveAddedItems.bind(this));
        else
            this._deletedNodesProvider.serializeSubsequentItems(count, didReceiveDeletedItems.bind(this, []));
    },

    sortAndRewind: function(comparator, callback)
    {
        this._serializedItemsCount = 0;

        function afterSort(ignored)
        {
            this._deletedNodesProvider.sortAndRewind(comparator, callback);
        }
        this._addedNodesProvider.sortAndRewind(comparator, afterSort.bind(this));
    }
};

/**
 * @constructor
 * @extends {WebInspector.HeapSnapshotGridNode}
 */
WebInspector.HeapSnapshotDiffNode = function(tree, className, diffForClass)
{
    WebInspector.HeapSnapshotGridNode.call(this, tree, true,
        WebInspector.HeapSnapshotDiffNode._createProvider(tree, className, diffForClass.deletedIndexes, diffForClass.addedCount, diffForClass.removedCount));
    this._name = className;

    this._addedCount = diffForClass.addedCount;
    this._removedCount = diffForClass.removedCount;
    this._countDelta = diffForClass.countDelta;
    this._addedSize = diffForClass.addedSize;
    this._removedSize = diffForClass.removedSize;
    this._sizeDelta = diffForClass.sizeDelta;
}

WebInspector.HeapSnapshotDiffNode._createProvider = function(tree, className, deletedIndexes, addedCount, removedCount)
{
    var result =  new WebInspector.HeapSnapshotDiffNodesProvider(
        tree.snapshot.createAddedNodesProvider(tree.baseSnapshot.uid, className),
        tree.baseSnapshot.createDeletedNodesProvider(deletedIndexes),
        addedCount, removedCount);
    return result;
}

WebInspector.HeapSnapshotDiffNode.prototype = {
    _createChildNode: function(item)
    {
        if (item.isAddedNotRemoved)
            return new WebInspector.HeapSnapshotInstanceNode(this._dataGrid, null, this._dataGrid.snapshot, item);
        else
            return new WebInspector.HeapSnapshotInstanceNode(this._dataGrid, this._dataGrid.baseSnapshot, null, item);
    },

    _childHashForEntity: function(node)
    {
        return node.id;
    },

    _childHashForNode: function(childNode)
    {
        return childNode.snapshotNodeId;
    },

    comparator: function()
    {
        var sortAscending = this._dataGrid.sortOrder === "ascending";
        var sortColumnIdentifier = this._dataGrid.sortColumnIdentifier;
        var sortFields = {
            object: ["id", sortAscending, "selfSize", false],
            addedCount: ["selfSize", sortAscending, "id", true],
            removedCount: ["selfSize", sortAscending, "id", true],
            countDelta: ["selfSize", sortAscending, "id", true],
            addedSize: ["selfSize", sortAscending, "id", true],
            removedSize: ["selfSize", sortAscending, "id", true],
            sizeDelta: ["selfSize", sortAscending, "id", true]
        }[sortColumnIdentifier];
        return WebInspector.HeapSnapshotFilteredOrderedIterator.prototype.createComparator(sortFields);
    },

    _signForDelta: function(delta)
    {
        if (delta === 0)
            return "";
        if (delta > 0)
            return "+";
        else
            return "\u2212";  // Math minus sign, same width as plus.
    },

    get data()
    {
        var data = {object: this._name};

        data["addedCount"] = Number.withThousandsSeparator(this._addedCount);
        data["removedCount"] = Number.withThousandsSeparator(this._removedCount);
        data["countDelta"] = this._signForDelta(this._countDelta) + Number.withThousandsSeparator(Math.abs(this._countDelta));
        data["addedSize"] = Number.withThousandsSeparator(this._addedSize);
        data["removedSize"] = Number.withThousandsSeparator(this._removedSize);
        data["sizeDelta"] = this._signForDelta(this._sizeDelta) + Number.withThousandsSeparator(Math.abs(this._sizeDelta));

        return data;
    }
};

WebInspector.HeapSnapshotDiffNode.prototype.__proto__ = WebInspector.HeapSnapshotGridNode.prototype;

/**
 * @constructor
 * @extends {WebInspector.HeapSnapshotGenericObjectNode}
 */
WebInspector.HeapSnapshotDominatorObjectNode = function(tree, node)
{
    WebInspector.HeapSnapshotGenericObjectNode.call(this, tree, node,
        tree.snapshot.createNodesProviderForDominator(node.nodeIndex));
    this.updateHasChildren();
};

WebInspector.HeapSnapshotDominatorObjectNode.prototype = {
    _createChildNode: function(item)
    {
        return new WebInspector.HeapSnapshotDominatorObjectNode(this._dataGrid, item);
    },

    _childHashForEntity: function(node)
    {
        return node.id;
    },

    _childHashForNode: function(childNode)
    {
        return childNode.snapshotNodeId;
    },

    comparator: function()
    {
        var sortAscending = this._dataGrid.sortOrder === "ascending";
        var sortColumnIdentifier = this._dataGrid.sortColumnIdentifier;
        var sortFields = {
            object: ["id", sortAscending, "retainedSize", false],
            shallowSize: ["selfSize", sortAscending, "id", true],
            retainedSize: ["retainedSize", sortAscending, "id", true]
        }[sortColumnIdentifier];
        return WebInspector.HeapSnapshotFilteredOrderedIterator.prototype.createComparator(sortFields);
    },

    _emptyData: function()
    {
        return {};
    }
};

WebInspector.HeapSnapshotDominatorObjectNode.prototype.__proto__ = WebInspector.HeapSnapshotGenericObjectNode.prototype;
