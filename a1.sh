#!/bin/bash
# This is a simple product management script using a text file acting as a database.

while true; do
    echo "1. CREATE PRODUCT DATABASE"
    echo "2. INSERT PRODUCT"
    echo "3. SEARCH PRODUCT"
    echo "4. DISPLAY PRODUCT DATABASE"
    echo "5. DELETE PRODUCT RECORD"
    echo "6. UPDATE PRODUCT RECORD"
    echo "7. EXIT"
    echo "Enter your choice:"
    read d

    case $d in
    1)
        echo "Enter the name of the product database:"
        read db

        # Check if the product database file exists
        if [ -f "$db" ]; then
            echo "$db exists"
        else
            # If the file does not exist, create it
            touch $db
            echo "Product database created successfully!"
        fi
        ;;

    2)
        echo "Enter the name of the product database in which you have to insert:"
        read db

        # Gather product details from the user
        echo "Enter the product ID:"
        read pd_id
        echo "Enter the product name:"
        read pd_name
        echo "Enter the product price:"
        read pd_price

        # Construct a record and append it to the database file
        record="$pd_id $pd_name $pd_price"
        echo $record >> $db
        ;;

    3)
        echo "Enter the name of the product database in which you want to search:"
        read db

        # Search for a product with the specified product ID
        echo "Enter the product ID to search:"
        read search_id

        # Check if the product database file exists
        if [ -f "$db" ]; then
            # Search for the product with the given ID using grep
            data=$(grep "^$search_id " "$db")

            # Check if the search result is empty using -z
            if [ -z "$data" ]; then
                echo "Product not found."
            else
                # Display the product details
                echo "Product found:"
                echo "Product ID | Product Name | Product Price"
                echo "$data"
            fi
        else
            echo "$db does not exist."
        fi
        ;;

    4)
        echo "Enter the name of the product database to display:"
        read db

        # Check if the product database file exists
        if [ -f "$db" ]; then
            # Display the content of the database file using cat
            cat $db
        else
            echo "$db does not exist."
        fi
        ;;

    5)
        echo "Enter the name of the product database from which you want to delete a record:"
        read db

        # Delete a product with the specified product ID
        echo "Enter the product ID to delete:"
        read delete_id

        # Check if the product database file exists
        if [ -f "$db" ]; then
            # Check if the product with the given ID exists in the database using grep -q
            if grep -q "^$delete_id " "$db"; then
                # Delete the product record using grep -v and overwrite the original database file
                grep -v "^$delete_id " "$db" > temp_db && mv temp_db "$db"
                echo "Product with ID $delete_id has been deleted."
            else
                echo "Product not found."
            fi
        else
            echo "$db does not exist."
        fi
        ;;

    6)
        echo "Enter the name of the product database in which you want to update a record:"
        read db

        # Update a product with the specified product ID
        echo "Enter the product ID to update:"
        read update_id

        # Check if the product database file exists
        if [ -f "$db" ]; then
            # Check if the product with the given ID exists in the database using grep -q
            if grep -q "^$update_id " "$db"; then
                # Gather updated product details from the user
                echo "Enter the new product name:"
                read pd_name
                echo "Enter the new product price:"
                read pd_price

                # Update the product record using sed and overwrite the original database file
                sed -i "s/^$update_id .*/$update_id $pd_name $pd_price/" "$db"
                echo "Product with ID $update_id has been updated."
            else
                echo "Product not found."
            fi
        else
            echo "$db does not exist."
        fi
        ;;

    7)
        echo "Exiting..."
        exit 0
        ;;

    esac
done
